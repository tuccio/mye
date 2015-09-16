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

#define __MYE_RSM_DEFAULT_RESOLUTION     1024
#define __MYE_RSM_DEFAULT_CSM_SLICES     4
#define __MYE_RSM_DEFAULT_CSM_LOG_WEIGHT 0.35f

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
	m_light(nullptr),
	m_volumeConstraint(AABB::FromMinMax(0, 0)),
	m_vsm(false)
{
	SetResolution(__MYE_RSM_DEFAULT_RESOLUTION);
}

bool DX11ReflectiveShadowMap::Create(bool vsm)
{

	m_rsmZPrepass = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/rsm_zprepass.msh",
		nullptr,
		{ { "type", "program" } }
	);

	m_vsm = vsm;

	if (vsm)
	{

		m_rsm[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/rsm_vsm.msh",
			nullptr,
			{ { "type", "program" } }
		);

		m_rsm[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/rsm_difftex_vsm.msh",
			nullptr,
			{ { "type", "program" } }
		);

		m_rsmPSSM[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/rsm_pssm_vsm.msh",
			nullptr,
			{ { "type", "program" } }
		);

		m_rsmPSSM[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/rsm_pssm_difftex_vsm.msh",
			nullptr,
			{ { "type", "program" } }
		);

	}
	else
	{

		m_rsm[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/rsm.msh",
			nullptr,
			{ { "type", "program" } }
		);

		m_rsm[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/rsm_difftex.msh",
			nullptr,
			{ { "type", "program" } }
		);

		m_rsmPSSM[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/rsm_pssm.msh",
			nullptr,
			{ { "type", "program" } }
		);

		m_rsmPSSM[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/rsm_pssm_difftex.msh",
			nullptr,
			{ { "type", "program" } }
		);

	}
	
	if (m_rsm[0]->Load() &&
	    m_rsm[1]->Load() &&
	    m_rsmPSSM[0]->Load() &&
	    m_rsmPSSM[1]->Load() &&
	    m_rsmZPrepass->Load() &&
	    __CreateRenderTargets() &&
	    __CreateDepthBuffers() &&
	    __CreateConstantBuffers())
	{
		m_initialized = true;
	}

	return m_initialized;

}
void DX11ReflectiveShadowMap::Destroy(void)
{

	__DestroyRenderTargets();
	__DestroyDepthBuffers();
	__DestroyConstantBuffers();

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
	D3D11_VIEWPORT newViewport = { 0, 0, m_resolution, m_resolution, 0.f, 1.f };

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

	if (m_vsm)
	{
		m_vsmMoments.ClearRenderTarget(Vector4(0));
	}

	m_depth.Clear();

	/* Find the tight frustum around the scene */

	SceneModule * scene  = Game::GetSingleton().GetSceneModule();
	Camera      * camera = scene->GetCamera();

	m_pssmSlices = __CSMComputeSplitsDepths(camera->GetNearClipDistance(), camera->GetFarClipDistance());

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

	m_cropMatrixCBuffer.SetSubData(&m_cropMatrix[0], 0, sizeof(Matrix4) * m_csmSplits);

	ID3D11RenderTargetView * renderTargets[4] =
	{
		m_position.GetRenderTargetView(),
		m_normal.GetRenderTargetView(),
		m_flux.GetRenderTargetView(),
		m_vsmMoments.GetRenderTargetView()
	};

	MakeLightBuffer(m_lightCBuffer, light);

	m_lightCBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);
	
	
	DX11RasterizerState cullState({ false, CullMode::NONE });

	cullState.Use();

	bool cascaded = m_csmSplits > 1;

	DX11ShaderProgramPointer * rsmShader;

	if (cascaded)
	{
		m_cropMatrixCBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 1);
		rsmShader = m_rsmPSSM;
	}
	else
	{
		shadowProjMatrix = m_cropMatrix[0] * shadowProjMatrix;
		rsmShader = m_rsm;
	}

	m_rsmZPrepass->Use();

	DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::ON);

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0,
	                                                                     nullptr,
	                                                                     m_depth.GetDepthStencilView());


	for (GameObject * object : casters)
	{

		RenderComponent    * rc = object->GetRenderComponent();
		TransformComponent * tc = object->GetTransformComponent();


		GPUBufferPointer gpuBuffer = rc->GetGPUBuffer();

		if (gpuBuffer && gpuBuffer->Load())
		{

			MakeTransformBuffer(m_transformCBuffer, tc->GetWorldMatrix() * rc->GetModelMatrix(), shadowViewMatrix, shadowProjMatrix);

			m_transformCBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);

			DX11VertexBufferPointer vertexBuffer = Resource::StaticCast<DX11VertexBuffer>(gpuBuffer);

			vertexBuffer->Bind();

			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (cascaded)
			{
				DX11Device::GetSingleton().GetImmediateContext()->DrawInstanced(vertexBuffer->GetVerticesCount(), m_csmSplits, 0, 0);
			}
			else
			{
				DX11Device::GetSingleton().GetImmediateContext()->Draw(vertexBuffer->GetVerticesCount(), 0);
			}

			vertexBuffer->Unbind();

		}

	}

	DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::LOOKUP);

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets((m_vsm ? 4 : 3),
	                                                                     renderTargets,
	                                                                     m_depth.GetDepthStencilView());

	int currentState = -1;

	for (GameObject * object : casters)
	{

		RenderComponent    * rc = object->GetRenderComponent();
		TransformComponent * tc = object->GetTransformComponent();


		GPUBufferPointer gpuBuffer = rc->GetGPUBuffer();

		if (gpuBuffer && gpuBuffer->Load())
		{

			DX11TexturePointer difftex = Resource::StaticCast<DX11Texture>(rc->GetDiffuseTexture());

			if (difftex && difftex->Load())
			{

				if (currentState != 1)
				{
					rsmShader[1]->Use();
					currentState = 1;
				}

				difftex->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_DIFFUSE);

			}
			else if (currentState != 0)
			{
				rsmShader[0]->Use();
				currentState = 0;
			}

			MakeTransformBuffer(m_transformCBuffer, tc->GetWorldMatrix() * rc->GetModelMatrix(), shadowViewMatrix, shadowProjMatrix);
			MakeMaterialBuffer(m_materialCBuffer, rc->GetMaterial());

			m_transformCBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
			m_materialCBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_MATERIAL);

			DX11VertexBufferPointer vertexBuffer = Resource::StaticCast<DX11VertexBuffer>(gpuBuffer);

			vertexBuffer->Bind();

			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			
			if (cascaded)
			{
				DX11Device::GetSingleton().GetImmediateContext()->DrawInstanced(vertexBuffer->GetVerticesCount(), m_csmSplits, 0, 0);
			}
			else
			{
				DX11Device::GetSingleton().GetImmediateContext()->Draw(vertexBuffer->GetVerticesCount(), 0);
			}

			vertexBuffer->Unbind();

		}

	}

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

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

	AABB cropAABB = (hCastersAABB ^ hReceiversAABB) ^ hFrustumAABB + m_volumeConstraint;

	//AABB cropAABB = hCastersAABB ^ hFrustumAABB + m_volumeConstraint;

	Vector3 cropAABBMax = cropAABB.GetMaximum();
	Vector3 cropAABBMin = cropAABB.GetMinimum();

	Vector2 maxXY = cropAABBMax.xy() + padding;
	Vector2 minXY = cropAABBMin.xy() - padding;

	Real minZ = Min(hCastersAABB.GetMinimum().z(),   hFrustumAABB.GetMinimum().z()) - padding;
	Real maxZ = Min(hReceiversAABB.GetMaximum().z(), hFrustumAABB.GetMaximum().z()) + padding;

	Vector2 S = 2.f / (maxXY - minXY);
	Vector2 O = - .5f * S * (maxXY + minXY);

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
	m_normal.SetParametersList(arrayParams);
	m_flux.SetParametersList(arrayParams);

	m_vsmMoments.SetParametersList(arrayParams);

	return (m_position.Create(m_resolution, m_resolution, DataFormat::HALF4) &&
	        m_normal.Create(m_resolution,   m_resolution, DataFormat::HALF4) &&
	        m_flux.Create(m_resolution,     m_resolution, DataFormat::HALF4) &&
			(!m_vsm || m_vsmMoments.Create(m_resolution, m_resolution, DataFormat::FLOAT2)));

}

void DX11ReflectiveShadowMap::__DestroyRenderTargets(void)
{

	m_position.Destroy();
	m_flux.Destroy();
	m_normal.Destroy();

	if (m_vsm)
	{
		m_vsmMoments.Destroy();
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

bool DX11ReflectiveShadowMap::__CreateConstantBuffers(void)
{

	return m_transformCBuffer.Create(sizeof(detail::TransformBuffer)) &&
	       m_cropMatrixCBuffer.Create(sizeof(Matrix4) * __MYE_RSM_MAX_CSM_COUNT) &&
	       m_materialCBuffer.Create(sizeof(detail::MaterialBuffer)) &&
	       m_lightCBuffer.Create(sizeof(detail::LightBuffer));

}

void DX11ReflectiveShadowMap::__DestroyConstantBuffers(void)
{

	m_transformCBuffer.Destroy();
	m_cropMatrixCBuffer.Destroy();
	m_materialCBuffer.Destroy();
	m_lightCBuffer.Destroy();

}