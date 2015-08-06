#include "DX11ReflectiveShadowMap.h"

#include "DX11ConstantBuffer.h"
#include "DX11VertexBuffer.h"
#include "DX11Utils.h"
#include "DX11RasterizerState.h"

#include "detail/ShadersBuffers.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include <mye/math/Geometry.h>

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::math;

#define __MYE_RSM_DEFAULT_RESOLUTION 1024
#define __MYE_RSM_DEFAULT_CSM_SLICES 4
#define __MYE_RSM_DEFAULT_CSM_LOG_WEIGHT 0.65f

template <typename Iterator>
static AABB MakeAABB(Iterator begin, Iterator end)
{
	
	if (begin != end)
	{

		AABB aabb = (*begin)->GetAABB();

		while (++begin != end)
		{
			aabb = aabb + (*begin)->GetAABB();
		}

		return aabb;

	}

	return AABB();

}

DX11ReflectiveShadowMap::DX11ReflectiveShadowMap(void) :
	m_depth({ 0, 0, true, __MYE_RSM_DEFAULT_CSM_SLICES }),
	m_initialized(false),
	m_csmSplits(__MYE_RSM_DEFAULT_CSM_SLICES),
	m_csmLogWeight(__MYE_RSM_DEFAULT_CSM_LOG_WEIGHT),
	m_light(nullptr)
{
	SetResolution(__MYE_RSM_DEFAULT_RESOLUTION);
}

bool DX11ReflectiveShadowMap::Create(void)
{

	VertexDeclaration rsmVD({
		VertexAttribute(VertexAttributeSemantic::POSITION,  DataFormat::FLOAT3),
		VertexAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2),
		VertexAttribute(VertexAttributeSemantic::NORMAL,    DataFormat::FLOAT3)
	});

	auto rsmILV = MakeInputElementVector(rsmVD);

	m_rsmVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/rsm_vs.cso",
		nullptr,
		Parameters({
				{ "type", "vertex" },
				{ "precompiled", "true" },
				{ "inputLayoutVector", PointerToString(&rsmILV) }
		})
	);

	m_singlePS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/rsm_ps.cso",
		nullptr,
		Parameters({
				{ "type", "pixel" },
				{ "precompiled", "true" },
		})
	);

	m_pssmGS = ResourceTypeManager::GetSingleton().CreateResource<DX11GeometryShader>(
		"DX11Shader",
		"./shaders/rsm_pssm_gs.cso",
		nullptr,
		Parameters({
				{ "type", "geometry" },
				{ "precompiled", "true" },
		})
	);

	m_pssmVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/rsm_pssm_vs.cso",
		nullptr,
		Parameters({
				{ "type", "vertex" },
				{ "precompiled", "true" },
				{ "inputLayoutVector", PointerToString(&rsmILV) }
		})
	);
	
	if (m_rsmVS->Load() &&
		m_singlePS->Load() &&
		m_pssmGS->Load() &&
		m_pssmVS->Load() &&
		__CreateRenderTargets() &&
		__CreateDepthBuffers())
	{
		m_initialized = true;
	}

	return m_initialized;

}
void DX11ReflectiveShadowMap::Destroy(void)
{

	__DestroyRenderTargets();
	__DestroyDepthBuffers();

	m_initialized = false;

}

void DX11ReflectiveShadowMap::SetResolution(int resolution)
{
	m_resolution = resolution;

	if (m_initialized)
	{

		__DestroyRenderTargets();
		__CreateRenderTargets();

		m_depth.Resize(resolution, resolution);

	}

}

void DX11ReflectiveShadowMap::SetCSMSplits(int splits)
{

	m_csmSplits = splits;

	if (m_initialized)
	{

		__DestroyRenderTargets();
		__CreateRenderTargets();

		m_depth.ResizeArray(splits);

	}

}

void DX11ReflectiveShadowMap::Render(Light * light)
{

	D3D11_VIEWPORT oldViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	D3D11_VIEWPORT newViewport = { 0, 0, m_resolution, m_resolution, 0.0f, 1.0f };

	unsigned int numViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

	DX11Device::GetSingleton().GetImmediateContext()->RSGetViewports(&numViewports, oldViewports);
	DX11Device::GetSingleton().GetImmediateContext()->RSSetViewports(1, &newViewport);

	m_light = light;

	auto type = light->GetType();

	switch (type)
	{

	case LightType::DIRECTIONAL:
		__RenderDirectionalLight(light);
		break;

	}

	DX11Device::GetSingleton().GetImmediateContext()->RSSetViewports(numViewports, oldViewports);

}

void DX11ReflectiveShadowMap::Bind(DX11PipelineStage stage, int slice)
{

	m_depth.Bind(stage,    __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);
	m_position.Bind(stage, __MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
	m_normal.Bind(stage,   __MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
	m_flux.Bind(stage,     __MYE_DX11_TEXTURE_SLOT_RSMFLUX);

}

void DX11ReflectiveShadowMap::Unbind(void)
{
	m_depth.Unbind();
	m_position.Unbind();
	m_normal.Unbind();
	m_flux.Unbind();
}

void DX11ReflectiveShadowMap::__RenderDirectionalLight(Light * light)
{

	/* Clear buffers */

	m_position.ClearRenderTarget(Vector4(0));
	m_normal.ClearRenderTarget(Vector4(0));
	m_flux.ClearRenderTarget(Vector4(0));

	m_depth.Clear();

	/* Find the tight frustum around the scene */

	SceneModule * scene  = Game::GetSingleton().GetSceneModule();
	Camera      * camera = scene->GetCamera();

	auto casters   = __CSMFindPotentialCasters(light->GetDirection(), camera->GetFrustum());
	auto receivers = scene->GetVisibleObjects(*camera);

	if (casters.empty() || receivers.empty())
	{
		return;
	}

	Camera lightCamera;
	lightCamera.LookAt(Vector3(0), Vector3(0, 1, 0), light->GetDirection());

	Matrix4 shadowViewMatrix = lightCamera.GetViewMatrix();
	Matrix4 shadowProjMatrix = OrthographicProjectionD3DLH(2.f, 2.f, 0.f, 1.f);

	m_lightSpaceTransform = shadowProjMatrix * shadowViewMatrix;

	AABB castersAABB   = MakeAABB(casters.begin(), casters.end());
	AABB receiversAABB = MakeAABB(receivers.begin(), receivers.end());

	AABB hCastersAABB   = castersAABB.TransformAffine(m_lightSpaceTransform);
	AABB hReceiversAABB = receiversAABB.TransformAffine(m_lightSpaceTransform);

	m_pssmSlices = __CSMComputeSplitsDepths(camera->GetNearClipDistance(), camera->GetFarClipDistance());

	// Texel padding for the crop matrix to allow the calculation of derivatives on the
	// slice border (necessary for slope scaled bias calculation)
	Real cropPadding = 0.f;

	for (int sliceIndex = 0; sliceIndex < m_csmSplits; sliceIndex++)
	{

		Camera sliceCamera = *camera;

		sliceCamera.SetNearClipDistance(m_pssmSlices[sliceIndex].near);
		sliceCamera.SetFarClipDistance(m_pssmSlices[sliceIndex].far);

		Frustum sliceFrustum = sliceCamera.GetFrustum();

		m_cropMatrix[sliceIndex] = __CSMCropMatrix(m_lightSpaceTransform, sliceFrustum, hCastersAABB, hReceiversAABB, cropPadding);

	}

	ID3D11RenderTargetView * renderTargets[3] =
	{
		m_position.GetRenderTargetView(),
		m_normal.GetRenderTargetView(),
		m_flux.GetRenderTargetView()
	};

	DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::ON);
	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(3,
	                                                   renderTargets,
	                                                   m_depth.GetDepthStencilView());

	DX11ConstantBuffer transformCBuffer;
	DX11ConstantBuffer cropMatrixCBuffer;
	DX11ConstantBuffer materialCBuffer;
	DX11ConstantBuffer lightCBuffer;

	transformCBuffer.Create(sizeof(detail::TransformBuffer));
	cropMatrixCBuffer.Create(sizeof(Matrix4) * m_csmSplits, &m_cropMatrix[0]);
	materialCBuffer.Create(sizeof(detail::MaterialBuffer));

	detail::LightBuffer lightBuffer;

	lightBuffer.color     = Vector4f(light->GetColor(), 1);
	lightBuffer.direction = Vector4f(light->GetDirection(), 1);
	lightBuffer.intensity = light->GetIntensity();

	lightCBuffer.Create(sizeof(detail::LightBuffer), &lightBuffer);
	lightCBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

	cropMatrixCBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 1);

	m_pssmVS->Use();
	m_pssmGS->Use();
	m_singlePS->Use();
	
	DX11RasterizerState cullState({ false, CullMode::NONE });

	cullState.Use();

	for (GameObject * object : casters)
	{

		RenderComponent * rc = object->GetRenderComponent();

		if (rc)
		{

			TransformComponent * tc = object->GetTransformComponent();

			//MeshPointer mesh = rc->GetMesh();

			GPUBufferPointer gpuBuffer = rc->GetGPUBuffer();

			//if (mesh && mesh->Load())
			if (gpuBuffer && gpuBuffer->Load())
			{

				detail::TransformBuffer transformBuffer;

				transformBuffer.world               = tc->GetWorldMatrix() * rc->GetModelMatrix();
				transformBuffer.worldView           = shadowViewMatrix * transformBuffer.world;
				transformBuffer.worldViewProjection = m_lightSpaceTransform * transformBuffer.world;

				transformCBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
				transformCBuffer.SetData(&transformBuffer);

				MaterialPointer material = rc->GetMaterial();

				detail::MaterialBuffer materialBuffer;

				materialBuffer.diffuseColor  = material->GetDiffuseColor();
				materialBuffer.specularColor = material->GetSpecularColor();
				materialBuffer.specular      = material->GetSpecular();
				materialBuffer.metallic      = material->GetMetallic();
				materialBuffer.roughness     = material->GetRoughness();

				materialCBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_MATERIAL);
				materialCBuffer.SetData(&materialBuffer);

				//DX11VertexBuffer vertexBuffer(nullptr, "", nullptr, m_device);
				DX11VertexBufferPointer vertexBuffer = Resource::StaticCast<DX11VertexBuffer>(gpuBuffer);

				//vertexBuffer.Create(mesh.get());
				vertexBuffer->Bind();

				DX11Device::GetSingleton().GetImmediateContext()->
					IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				DX11Device::GetSingleton().GetImmediateContext()->
					DrawInstanced(vertexBuffer->GetVerticesCount(), m_csmSplits, 0, 0);

				vertexBuffer->Unbind();

			}

		}

	}

	m_pssmVS->Dispose();
	m_pssmGS->Dispose();
	m_singlePS->Dispose();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

	transformCBuffer.Destroy();
	cropMatrixCBuffer.Destroy();
	materialCBuffer.Destroy();
	lightCBuffer.Destroy();

}

std::vector<DX11ReflectiveShadowMap::PSSMSlice> DX11ReflectiveShadowMap::__CSMComputeSplitsDepths(Real near, Real far)
{

	std::vector<PSSMSlice> slices(m_csmSplits);

	Real ratio         = far / near;
	Real inverseSlices = 1.0f / m_csmSplits;

	slices[0].near = near;

	for (int i = 1; i < m_csmSplits; i++)
	{

		Real splitRatio   = i * inverseSlices;

		Real logComponent = near * std::pow(ratio, splitRatio);
		Real linComponent = near + (far - near) * splitRatio;

		slices[i].near    = m_csmLogWeight * logComponent + (1 - m_csmLogWeight) * linComponent;
		//slices[i].near    = linComponent;
		slices[i - 1].far = slices[i].near;

	}

	slices[m_csmSplits - 1].far = far;

	return slices;

}

Matrix4 DX11ReflectiveShadowMap::__CSMCropMatrix(const Matrix4 & shadowViewProjMatrix,
                                                 const Frustum & sliceFrustum,
												 const AABB    & hCastersAABB,
                                                 const AABB    & hReceiversAABB,
												 Real            padding)
{

	auto frustumCorners = sliceFrustum.GetCorners();

	Vector3 hFrustumCorners[8];

	std::transform(frustumCorners.begin(), frustumCorners.end(),
				   hFrustumCorners,
	               [&shadowViewProjMatrix] (const Vector3 & v) { return (shadowViewProjMatrix * Vector4(v, 1)).xyz(); });

	AABB hFrustumAABB = BoundingAABB(hFrustumCorners, hFrustumCorners + 8);

	AABB cropAABB = (hCastersAABB ^ hReceiversAABB) ^ hFrustumAABB;

	Vector3 cropAABBMax = cropAABB.GetMaximum();
	Vector3 cropAABBMin = cropAABB.GetMinimum();

	Vector2 maxXY = cropAABBMax.xy() + padding;
	Vector2 minXY = cropAABBMin.xy() - padding;

	Real minZ = Min(hCastersAABB.GetMinimum().z(),   hFrustumAABB.GetMinimum().z()) - padding;
	Real maxZ = Min(hReceiversAABB.GetMaximum().z(), hFrustumAABB.GetMaximum().z()) + padding;

	Vector2 S = 2.f / (maxXY - minXY);
	Vector2 O = -.5f * S * (maxXY + minXY);

	Real SZ = 1.f / (maxZ - minZ);
	Real OZ = - minZ * SZ;

	Matrix4 cropMatrix = Matrix4(1);

	cropMatrix.m00() = S.x();
	cropMatrix.m03() = O.x();
	
	cropMatrix.m11() = S.y();
	cropMatrix.m13() = O.y();

	cropMatrix.m22() = SZ;
	cropMatrix.m23() = OZ;

	return cropMatrix;

}

GameObjectsList DX11ReflectiveShadowMap::__CSMFindPotentialCasters(const Vector3 & lightDirection, const Frustum & cameraFrustum)
{

	SceneModule * scene  = Game::GetSingleton().GetSceneModule();

	AABB    sceneAABB     = scene->GetAABB();
	AABB    cameraAABB    = BoundingAABB(cameraFrustum);

	Vector3 sceneAABBmin  = sceneAABB.GetMinimum();
	Vector3 sceneAABBmax  = sceneAABB.GetMaximum();

	Vector3 cameraAABBmin = cameraAABB.GetMinimum();
	Vector3 cameraAABBmax = cameraAABB.GetMaximum();

	Vector3 castersAABBmin, castersAABBmax;

	if (lightDirection.x() < 0.f)
	{
		castersAABBmax.x() = sceneAABBmax.x();
		castersAABBmin.x() = cameraAABBmin.x();
	}
	else
	{
		castersAABBmax.x() = cameraAABBmax.x();
		castersAABBmin.x() = sceneAABBmin.x();
	}

	if (lightDirection.y() < 0.f)
	{
		castersAABBmax.y() = sceneAABBmax.y();
		castersAABBmin.y() = cameraAABBmin.y();
	}
	else
	{
		castersAABBmax.y() = cameraAABBmax.y();
		castersAABBmin.y() = sceneAABBmin.y();
	}

	if (lightDirection.z() < 0.f)
	{
		castersAABBmax.z() = sceneAABBmax.z();
		castersAABBmin.z() = cameraAABBmin.z();
	}
	else
	{
		castersAABBmax.z() = cameraAABBmax.z();
		castersAABBmin.z() = sceneAABBmin.z();
	}

	AABB potentialCastersAABB = AABB::FromMinMax(castersAABBmin, castersAABBmax);

	return scene->FindObjects(potentialCastersAABB);

}

DX11ReflectiveShadowMap::PSSMSlice DX11ReflectiveShadowMap::__CSMFindTightPlanes(Camera * camera, const AABB & aabb)
{

	auto corners = aabb.GetCorners();
	Vector3 tCorners[8];

	Matrix4 view = camera->GetViewMatrix();

	std::transform(corners.begin(), corners.end(),
	               tCorners,
	               [&view] (const Vector3 & v) { return (view * Vector4(v, 1)).xyz(); });

	AABB tAABB = BoundingAABB(tCorners, tCorners + 8);

	PSSMSlice planes;

	planes.near = tAABB.GetMinimum().z();
	planes.far  = tAABB.GetMaximum().z();

	return planes;

}

bool DX11ReflectiveShadowMap::__CreateRenderTargets(void)
{

	Parameters rtParams({ { "renderTarget", "true" } });

	Parameters arrayParams = { { "renderTarget", "true" }, { "slices", ToString(m_csmSplits) } };

	m_position.SetParametersList(arrayParams);
	m_flux.SetParametersList(arrayParams);
	m_normal.SetParametersList(arrayParams);

	return (m_position.Create(m_resolution, m_resolution, DataFormat::FLOAT4) &&
	        m_normal.Create(m_resolution,   m_resolution, DataFormat::FLOAT4) &&
	        m_flux.Create(m_resolution,     m_resolution, DataFormat::FLOAT4));

}

void DX11ReflectiveShadowMap::__DestroyRenderTargets(void)
{

	for (int i = 0; i < m_csmSplits; i++)
	{
		m_position.Destroy();
		m_flux.Destroy();
		m_normal.Destroy();
	}

}

bool DX11ReflectiveShadowMap::__CreateDepthBuffers(void)
{

	m_depth = DX11DepthBufferConfiguration { m_resolution, m_resolution, true, m_csmSplits };

	return m_depth.Create();

}

void DX11ReflectiveShadowMap::__DestroyDepthBuffers(void)
{
	m_depth.Destroy();
}