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
#define __MYE_RSM_DEFAULT_CSM_SLICES 3
#define __MYE_RSM_DEFAULT_CSM_LOG_WEIGHT 0.35f

DX11ReflectiveShadowMap::DX11ReflectiveShadowMap(DX11Device & device) :
	m_position(nullptr, "", nullptr, device),
	m_normal(nullptr, "", nullptr, device),
	m_flux(nullptr, "", nullptr, device),
	m_depth({ &device, 0, 0, true, __MYE_RSM_DEFAULT_CSM_SLICES }),
	m_device(device),
	m_initialized(false),
	m_csmSlices(__MYE_RSM_DEFAULT_CSM_SLICES),
	m_csmLogWeight(__MYE_RSM_DEFAULT_CSM_LOG_WEIGHT),
	m_positionArray(nullptr, "", nullptr, device)
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

void DX11ReflectiveShadowMap::Render(Light * light)
{

	D3D11_VIEWPORT oldViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	D3D11_VIEWPORT newViewport = { 0, 0, m_resolution, m_resolution, 0.0f, 1.0f };

	unsigned int numViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

	m_device.GetImmediateContext()->RSGetViewports(&numViewports, oldViewports);
	m_device.GetImmediateContext()->RSSetViewports(1, &newViewport);

	auto type = light->GetType();

	switch (type)
	{

	case LightType::DIRECTIONAL:
		__RenderDirectionalLight(light);
		break;

	}

	m_device.GetImmediateContext()->RSSetViewports(numViewports, oldViewports);

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

	auto casters = __CSMFindPotentialCasters(light->GetDirection(), camera->GetFrustum());

	if (casters.empty())
	{
		return;
	}

	AABB tightCastersAABB = casters.front()->GetAABB();

	for (GameObject * caster : casters)
	{
		tightCastersAABB = tightCastersAABB + caster->GetAABB();
	}

	PSSMSlice tightPlanes = __CSMFindTightPlanes(camera, tightCastersAABB);

	/*Camera vCamera = *camera;
	
	if (tightPlanes.near < 0.f)
	{

		Vector3 camPosition  = vCamera.GetPosition();
		Vector3 camDirection = vCamera.Forward();

		vCamera.SetPosition(camPosition + tightPlanes.near * camDirection);
		vCamera.SetNearClipDistance(0.f);

	}

	vCamera.SetFarClipDistance(tightPlanes.far - tightPlanes.near);*/

	m_pssmSlices = __CSMComputeSplitsDepths(tightPlanes.near, tightPlanes.far);

	Camera lightCamera;

	lightCamera.LookAt(Vector3(0), Vector3(0, 1, 0), light->GetDirection());

	Matrix4 shadowViewMatrix = lightCamera.GetViewMatrix();
	Matrix4 shadowProjMatrix = OrthographicProjectionD3DLH(1.f, 1.f, 0.f, 1.f);

	m_lightSpaceTransform = shadowProjMatrix * shadowViewMatrix;

	for (int sliceIndex = 0; sliceIndex < m_csmSlices; sliceIndex++)
	{

		Camera sliceCamera = *camera;

		sliceCamera.SetNearClipDistance(m_pssmSlices[sliceIndex].near);
		sliceCamera.SetFarClipDistance(m_pssmSlices[sliceIndex].far);

		Frustum sliceFrustum = sliceCamera.GetFrustum();

		m_cropMatrix[sliceIndex] = __CSMCropMatrix(shadowViewMatrix, shadowProjMatrix, sliceFrustum, light->GetDirection());

	}

	ID3D11RenderTargetView * renderTargets[3] =
	{
		m_position.GetRenderTargetView(),
		m_normal.GetRenderTargetView(),
		m_flux.GetRenderTargetView()
	};

	m_device.SetDepthTest(DX11DepthTest::ON);
	m_device.GetImmediateContext()->OMSetRenderTargets(3,
	                                                   renderTargets,
	                                                   m_depth.GetDepthStencilView());

	DX11ConstantBuffer transformCBuffer(nullptr, "", nullptr, m_device);
	DX11ConstantBuffer cropMatrixCBuffer(nullptr, "", nullptr, m_device);
	DX11ConstantBuffer materialCBuffer(nullptr, "", nullptr, m_device);
	DX11ConstantBuffer lightCBuffer(nullptr, "", nullptr, m_device);

	transformCBuffer.Create(sizeof(detail::TransformBuffer));
	cropMatrixCBuffer.Create(sizeof(Matrix4) * m_csmSlices, &m_cropMatrix[0]);
	materialCBuffer.Create(sizeof(detail::MaterialBuffer));

	detail::LightBuffer lightBuffer;

	lightBuffer.color     = Vector4f(light->GetColor() * light->GetIntensity(), 1);
	lightBuffer.direction = Vector4f(light->GetDirection(), 1);

	lightCBuffer.Create(sizeof(detail::LightBuffer), &lightBuffer);
	lightCBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

	cropMatrixCBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 1);

	m_pssmVS->Use();
	m_pssmGS->Use();
	m_singlePS->Use();
	
	DX11RasterizerState backCull(m_device, { false, CullMode::BACK });

	backCull.Use();

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

				m_device.GetImmediateContext()->
					IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				m_device.GetImmediateContext()->
					DrawInstanced(vertexBuffer->GetVerticesCount(), m_csmSlices, 0, 0);

				vertexBuffer->Unbind();

			}

		}

	}

	m_pssmVS->Dispose();
	m_pssmGS->Dispose();
	m_singlePS->Dispose();

	m_device.GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

	transformCBuffer.Destroy();
	cropMatrixCBuffer.Destroy();
	materialCBuffer.Destroy();
	lightCBuffer.Destroy();

}

std::vector<DX11ReflectiveShadowMap::PSSMSlice> DX11ReflectiveShadowMap::__CSMComputeSplitsDepths(Real near, Real far)
{

	std::vector<PSSMSlice> slices(m_csmSlices);

	//Real ratio         = far / near;
	Real inverseSlices = 1.0f / m_csmSlices;

	slices[0].near = near;

	for (int i = 1; i < m_csmSlices; i++)
	{

		Real splitRatio   = i * inverseSlices;

		//Real logComponent = near * std::pow(ratio, splitRatio);
		Real linComponent = near + (far - near) * splitRatio;

		//slices[i].near    = m_csmLogWeight * logComponent + (1 - m_csmLogWeight) * linComponent;
		slices[i].near    = linComponent;
		slices[i - 1].far = slices[i].near;

	}

	slices[m_csmSlices - 1].far = far;

	return slices;

}

Matrix4 DX11ReflectiveShadowMap::__CSMCropMatrix(const Matrix4 & shadowViewMatrix,
                                                 const Matrix4 & shadowProjMatrix,
                                                 const Frustum & sliceFrustum,
												 const Vector3 & lightDirection)
{

	SceneModule * scene = SceneModule::GetSingletonPointer();

	scene->GetAABB();

	Matrix4 shadowViewProjMatrix = shadowProjMatrix * shadowViewMatrix;

	auto casters = __CSMFindPotentialCasters(lightDirection, sliceFrustum);

	AABB tightCastersAABB = casters.front()->GetAABB();

	for (GameObject * caster : casters)
	{
		tightCastersAABB = tightCastersAABB + caster->GetAABB();
	}

	AABB splitAABB = tightCastersAABB ^ BoundingAABB(sliceFrustum);

	auto corners = splitAABB.GetCorners();

	Vector3 hCorners[8];

	std::transform(corners.begin(), corners.end(),
	               hCorners,
	               [&shadowViewProjMatrix] (const Vector3 & v) { return (shadowViewProjMatrix * Vector4(v, 1)).xyz(); });

	AABB cropBB = BoundingAABB(hCorners, hCorners + 8);

	Vector3 cropBBMax = cropBB.GetMaximum();
	Vector3 cropBBMin = cropBB.GetMinimum();

	Real maxX = cropBBMax.x();
	Real maxY = cropBBMax.y();
	Real maxZ = cropBBMax.z();

	Real minX = cropBBMin.x();
	Real minY = cropBBMin.y();
	//Real minZ = 0.0f;
	Real minZ = cropBBMin.z();

	Vector2 maxXY = cropBBMax.xy();
	Vector2 minXY = cropBBMin.xy();

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

	if (-lightDirection.x() > 0.f)
	{
		castersAABBmax.x() = sceneAABBmax.x();
		castersAABBmin.x() = cameraAABBmin.x();
	}
	else
	{
		castersAABBmax.x() = cameraAABBmax.x();
		castersAABBmin.x() = sceneAABBmin.x();
	}

	if (-lightDirection.y() > 0.f)
	{
		castersAABBmax.y() = sceneAABBmax.y();
		castersAABBmin.y() = cameraAABBmin.y();
	}
	else
	{
		castersAABBmax.y() = cameraAABBmax.y();
		castersAABBmin.y() = sceneAABBmin.y();
	}

	if (-lightDirection.z() > 0.f)
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

	Parameters arrayParams = { { "renderTarget", "true" }, { "slices", ToString(m_csmSlices) } };

	m_position.SetParametersList(arrayParams);
	m_flux.SetParametersList(arrayParams);
	m_normal.SetParametersList(arrayParams);

	return (m_position.Create(m_resolution, m_resolution, DataFormat::FLOAT4) &&
	        m_normal.Create(m_resolution, m_resolution, DataFormat::FLOAT4) &&
	        m_flux.Create(m_resolution, m_resolution, DataFormat::FLOAT4));

}

void DX11ReflectiveShadowMap::__DestroyRenderTargets(void)
{

	for (int i = 0; i < m_csmSlices; i++)
	{
		m_position.Destroy();
		m_flux.Destroy();
		m_normal.Destroy();
	}

}

bool DX11ReflectiveShadowMap::__CreateDepthBuffers(void)
{

	m_depth = DX11DepthBufferConfiguration { &m_device, m_resolution, m_resolution, true, m_csmSlices };

	return m_depth.Create();

}

void DX11ReflectiveShadowMap::__DestroyDepthBuffers(void)
{
	m_depth.Destroy();
}