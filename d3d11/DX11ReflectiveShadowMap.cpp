#include "DX11ReflectiveShadowMap.h"

#include "DX11ConstantBuffer.h"
#include "DX11VertexBuffer.h"
#include "DX11Utils.h"

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
#define __MYE_RSM_DEFAULT_CSM_LOG_WEIGHT 0.45f

DX11ReflectiveShadowMap::DX11ReflectiveShadowMap(DX11Device & device, bool vsm) :
	/*m_position(nullptr, "", nullptr, device),
	m_normal(nullptr, "", nullptr, device),
	m_flux(nullptr, "", nullptr, device),
	m_vsm(nullptr, "", nullptr, device),
	m_depth({ &device, 0, 0, true }),*/
	m_device(device),
	m_varianceShadowMapping(vsm),
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

	m_rsmPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/rsm_ps.cso",
		nullptr,
		Parameters({
				{ "type", "pixel" },
				{ "precompiled", "true" },
		})
	);

	m_rsmVSMPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/rsm_vsm_ps.cso",
		nullptr,
		Parameters({
				{ "type", "pixel" },
				{ "precompiled", "true" },
		})
	);
	
	if (m_rsmVS->Load() &&
		m_rsmPS->Load() &&
		m_rsmVSMPS->Load() &&
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

		for (int i = 0; i < m_csmSlices; i++)
		{
			m_depth[i].Resize(resolution, resolution);
		}

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

	m_depth[slice].Bind(stage,    __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);
	m_position[slice].Bind(stage, __MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
	m_normal[slice].Bind(stage,   __MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
	m_flux[slice].Bind(stage,     __MYE_DX11_TEXTURE_SLOT_RSMFLUX);

	m_lastBoundSlice = slice;

}

void DX11ReflectiveShadowMap::Unbind(void)
{
	m_depth[m_lastBoundSlice].Unbind();
	m_position[m_lastBoundSlice].Unbind();
	m_normal[m_lastBoundSlice].Unbind();
	m_flux[m_lastBoundSlice].Unbind();
}

void DX11ReflectiveShadowMap::__RenderDirectionalLight(Light * light)
{

	SceneModule * scene = Game::GetSingleton().GetSceneModule();
	Camera      * camera = scene->GetCamera();

	Camera lightCamera;

	lightCamera.LookAt(Vector3(0), Vector3(0, 1, 0), light->GetDirection());

	std::vector<__CSMSlices> f = __CSMComputeSplitsDepths(camera->GetNearClipDistance(), camera->GetFarClipDistance());

	Matrix4 shadowViewMatrix = lightCamera.GetViewMatrix();
	Matrix4 shadowProjMatrix = Matrix4(1);

	Transform tmp(Vector3(0), lightCamera.GetOrientation(), Vector3(1));

	for (int sliceIndex = 0; sliceIndex < m_csmSlices; sliceIndex++)
	{

		Camera sliceCamera = *camera;

		sliceCamera.SetNearClipDistance(f[sliceIndex].near);
		sliceCamera.SetFarClipDistance(f[sliceIndex].far);

		Frustum sliceFrustum = sliceCamera.GetFrustum();

		Matrix4 cropMatrix = __CSMCropMatrix(shadowViewMatrix, shadowProjMatrix, sliceFrustum);

		m_lightSpaceTransform[sliceIndex] = cropMatrix * shadowProjMatrix * shadowViewMatrix;

		//auto objects = scene->GetVisibleObjects(sliceFrustum);
		auto objects = scene->GetObjectsList();

		ID3D11RenderTargetView * renderTargets[4] =
		{
			m_position[sliceIndex].GetRenderTargetView(),
			m_normal[sliceIndex].GetRenderTargetView(),
			m_flux[sliceIndex].GetRenderTargetView(),
			nullptr
		};

		m_position[sliceIndex].ClearRenderTarget(Vector4(0));
		m_normal[sliceIndex].ClearRenderTarget(Vector4(0));
		m_flux[sliceIndex].ClearRenderTarget(Vector4(0));

		m_depth[sliceIndex].Clear();

		int nRenderTargets;

		m_rsmVS->Use();

		if (m_varianceShadowMapping)
		{

			nRenderTargets = 4;

			renderTargets[3] = m_vsm[sliceIndex].GetRenderTargetView();
			m_vsm[sliceIndex].ClearRenderTarget(Vector4(0));

			m_rsmVSMPS->Use();

		}
		else
		{
			nRenderTargets = 3;
			m_rsmPS->Use();
		}		

		m_device.SetDepthTest(DX11DepthTest::ON);
		m_device.GetImmediateContext()->OMSetRenderTargets(nRenderTargets,
		                                                   renderTargets,
		                                                   m_depth[sliceIndex].GetDepthStencilView());

		DX11ConstantBuffer transformCBuffer(nullptr, "", nullptr, m_device);
		DX11ConstantBuffer materialCBuffer(nullptr, "", nullptr, m_device);
		DX11ConstantBuffer lightCBuffer(nullptr, "", nullptr, m_device);

		transformCBuffer.Create(sizeof(detail::TransformBuffer));
		materialCBuffer.Create(sizeof(detail::MaterialBuffer));

		detail::LightBuffer lightBuffer;

		lightBuffer.color     = Vector4f(light->GetColor() * light->GetIntensity(), 1);
		lightBuffer.direction = Vector4f(light->GetDirection(), 1);

		lightCBuffer.Create(sizeof(detail::LightBuffer), &lightBuffer);
		lightCBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

		for (GameObject * object : objects)
		{

			RenderComponent * rc = object->GetRenderComponent();

			if (rc)
			{

				TransformComponent * tc = object->GetTransformComponent();

				MeshPointer mesh = rc->GetMesh();

				if (mesh && mesh->Load())
				{

					detail::TransformBuffer transformBuffer;

					transformBuffer.world               = tc->GetWorldMatrix() * rc->GetModelMatrix();
					transformBuffer.worldView           = shadowViewMatrix * transformBuffer.world;
					transformBuffer.worldViewProjection = m_lightSpaceTransform[sliceIndex] * transformBuffer.world;

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

					DX11VertexBuffer vertexBuffer(nullptr, "", nullptr, m_device);

					vertexBuffer.Create(mesh.get());
					vertexBuffer.Bind();

					m_device.GetImmediateContext()->
						IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					m_device.GetImmediateContext()->
						Draw(vertexBuffer.GetVerticesCount(), 0);

					vertexBuffer.Unbind();
					vertexBuffer.Destroy();

				}

			}

		}


		m_device.GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

		if (m_varianceShadowMapping)
		{
			m_vsm[sliceIndex].GenerateMips();
		}

		transformCBuffer.Destroy();
		materialCBuffer.Destroy();
		lightCBuffer.Destroy();

	}

	//auto scene = Game::GetSingleton().GetSceneModule();

	//Camera * camera = scene->GetCamera();

	//auto viewFrustum = camera->GetFrustum();

	//auto viewFrustumCorners = viewFrustum.GetCorners();
	//Vector3 centroid = Vector3(0);

	//for (auto & corner : viewFrustumCorners)
	//{
	//	centroid = centroid + corner;
	//}

	//centroid = centroid / 8;

	//Vector3 lightViewPosition = centroid;
	//Vector3 lightDirection    = light->GetDirection();
	//Real    lightRange        = light->GetRange();

	//Camera lightCamera;


	//lightCamera.LookAt(lightViewPosition, Vector3(0, 1, 0), lightViewPosition + lightDirection);

	//Matrix4 lightView = lightCamera.GetViewMatrix();

	//std::vector<Vector3> lightFrustumCorners;

	//for (auto & corner : viewFrustumCorners)
	//{
	//	lightFrustumCorners.push_back(lightView * corner);
	//}

	//auto minMaxX = std::minmax_element(lightFrustumCorners.begin(),
	//								   lightFrustumCorners.end(),
	//								   [] (const Vector3 & a, const Vector3 & b) { return a.x() < b.x(); });

	//auto minMaxY = std::minmax_element(lightFrustumCorners.begin(),
	//								   lightFrustumCorners.end(),
	//								   [] (const Vector3 & a, const Vector3 & b) { return a.y() < b.y(); });

	//auto minMaxZ = std::minmax_element(lightFrustumCorners.begin(),
	//								   lightFrustumCorners.end(),
	//								   [] (const Vector3 & a, const Vector3 & b) { return a.z() < b.z(); });

	//Real l = - lightRange * minMaxX.second->x();
	//Real r = - lightRange * minMaxX.first->x();
	//Real b = lightRange * minMaxY.first->y();
	//Real t = lightRange * minMaxY.second->y();
	//Real n = lightRange * minMaxZ.first->z();
	//Real f = lightRange * minMaxZ.second->z();
	//
	//Matrix4 lightProjection = OrthographicProjection(l, r, b, t, n, f);
	//
	//m_lightSpaceTransform = lightProjection * lightView;

	//// TODO: Get light frustum and process only visible objects
	//auto objects = scene->GetObjectsList();

	//ID3D11RenderTargetView * renderTargets[] =
	//{
	//	m_position.GetRenderTargetView(),
	//	m_normal.GetRenderTargetView(),
	//	m_flux.GetRenderTargetView(),
	//	m_vsm.GetRenderTargetView()
	//};

	//int nRenderTargets;

	//m_rsmVS->Use();

	//if (m_varianceShadowMapping)
	//{
	//	m_rsmVSMPS->Use();
	//	m_vsm.ClearRenderTarget(Vector4(0));
	//	nRenderTargets = 4;
	//}
	//else
	//{
	//	m_rsmPS->Use();
	//	nRenderTargets = 3;
	//}

	//m_position.ClearRenderTarget(Vector4(0));
	//m_normal.ClearRenderTarget(Vector4(0));
	//m_flux.ClearRenderTarget(Vector4(0));

	//m_depth.Clear();

	//m_device.SetDepthTest(DX11DepthTest::ON);
	//m_device.GetImmediateContext()->OMSetRenderTargets(nRenderTargets,
	//												   renderTargets,
	//												   m_depth.GetDepthStencilView());

	//DX11ConstantBuffer transformCBuffer(nullptr, "", nullptr, m_device);
	//DX11ConstantBuffer materialCBuffer(nullptr, "", nullptr, m_device);
	//DX11ConstantBuffer lightCBuffer(nullptr, "", nullptr, m_device);

	//transformCBuffer.Create(sizeof(detail::TransformBuffer));
	//materialCBuffer.Create(sizeof(detail::MaterialBuffer));

	//detail::LightBuffer lightBuffer;

	//lightBuffer.color     = Vector4f(light->GetColor() * light->GetIntensity(), 1);
	//lightBuffer.direction = Vector4f(light->GetDirection(), 1);

	//lightCBuffer.Create(sizeof(detail::LightBuffer), &lightBuffer);
	//lightCBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

	//for (GameObject * object : objects)
	//{

	//	RenderComponent * rc = object->GetRenderComponent();

	//	if (rc)
	//	{

	//		TransformComponent * tc = object->GetTransformComponent();

	//		MeshPointer mesh = rc->GetMesh();

	//		if (mesh && mesh->Load())
	//		{

	//			detail::TransformBuffer transformBuffer;

	//			transformBuffer.world               = tc->GetWorldMatrix() * rc->GetModelMatrix();
	//			transformBuffer.worldView           = transformBuffer.world;
	//			transformBuffer.worldViewProjection = m_lightSpaceTransform * transformBuffer.world;

	//			transformCBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
	//			transformCBuffer.SetData(&transformBuffer);

	//			MaterialPointer material = rc->GetMaterial();

	//			detail::MaterialBuffer materialBuffer;

	//			materialBuffer.diffuseColor  = material->GetDiffuseColor();
	//			materialBuffer.specularColor = material->GetSpecularColor();
	//			materialBuffer.specular      = material->GetSpecular();
	//			materialBuffer.metallic      = material->GetMetallic();
	//			materialBuffer.roughness     = material->GetRoughness();

	//			materialCBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_MATERIAL);
	//			materialCBuffer.SetData(&materialBuffer);

	//			DX11VertexBuffer vertexBuffer(nullptr, "", nullptr, m_device);

	//			vertexBuffer.Create(mesh.get());
	//			vertexBuffer.Bind();

	//			m_device.GetImmediateContext()->
	//				IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//			m_device.GetImmediateContext()->
	//				Draw(vertexBuffer.GetVerticesCount(), 0);

	//			vertexBuffer.Unbind();
	//			vertexBuffer.Destroy();

	//		}

	//	}

	//}


	//m_device.GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

	//if (m_varianceShadowMapping)
	//{
	//	m_vsm.GenerateMips();
	//}

	//transformCBuffer.Destroy();
	//materialCBuffer.Destroy();
	//lightCBuffer.Destroy();

}

std::vector<DX11ReflectiveShadowMap::__CSMSlices> DX11ReflectiveShadowMap::__CSMComputeSplitsDepths(mye::math::Real near, mye::math::Real far)
{

	std::vector<__CSMSlices> slices(m_csmSlices);

	Real ratio         = far / near;
	Real inverseSlices = 1.0f / m_csmSlices;

	slices[0].near = near;

	for (int i = 1; i < m_csmSlices; i++)
	{

		Real splitRatio = i * inverseSlices;

		slices[i].near    = m_csmLogWeight * (near * std::pow(ratio, splitRatio)) * (1 - m_csmLogWeight) * (near + (far - near) * splitRatio);
		slices[i - 1].far = slices[i].near * 1.005f;

	}

	slices[m_csmSlices - 1].far = far;

	return slices;

}

Matrix4 DX11ReflectiveShadowMap::__CSMCropMatrix(const Matrix4 & shadowViewMatrix,
                                                 const Matrix4 & shadowProjMatrix,
                                                 const Frustum & sliceFrustum)
{

	Matrix4 shadowViewProjMatrix = shadowProjMatrix * shadowViewMatrix;

	auto corners = sliceFrustum.GetCorners();

	Vector3 hCorners[8];

	for (int i = 0; i < 8; i++)
	{
		Vector4 tCorner = shadowViewProjMatrix * Vector4(corners[i], 1);
		hCorners[i] = tCorner.xyz();
	}

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
	Vector2 O = -.5f * S *	(maxXY + minXY);

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

	//Vector3 hCorners[8];

	//for (int i = 0; i < 8; i++)
	//{
	//	hCorners[i] = (shadowViewMatrix * Vector4(corners[i], 1)).xyz();
	//}

	//auto minMaxZ = std::minmax_element(hCorners, hCorners + 8, [] (const Vector3 & a, const Vector3 & b) { return a.z() < b.z(); });

	//Real minZ = minMaxZ.first->z();
	//Real maxZ = minMaxZ.second->z();

	//Matrix4 shadowProjMatrix = OrthographicProjection(- 1.0f, 1.0f, - 1.0f, 1.0f, minZ, maxZ);

	//Matrix4 shadowViewProjMatrix = shadowProjMatrix * shadowViewMatrix;

	//for (int i = 0; i < 8; i++)
	//{
	//	Vector4 t = shadowViewProjMatrix * Vector4(corners[i], 1);
	//	hCorners[i] = t.xyz() / t.w();
	//}

	//auto minMaxX = std::minmax_element(hCorners, hCorners + 8, [] (const Vector3 & a, const Vector3 & b) { return a.x() < b.x(); });
	//auto minMaxY = std::minmax_element(hCorners, hCorners + 8, [] (const Vector3 & a, const Vector3 & b) { return a.y() < b.y(); });

	//Real minX = minMaxX.first->x();
	//Real maxX = minMaxX.second->x();

	//Real minY = minMaxY.first->y();
	//Real maxY = minMaxY.second->y();

	//Matrix4 cropMatrix = Matrix4(1);

	//Real Sx = 2.0f / (maxX - minX);
	//Real Sy = 2.0f / (maxY - minY);

	//Real Ox = - 0.5f * (maxX + minX) * Sx;
	//Real Oy = - 0.5f * (maxY + minY) * Sy;

	//cropMatrix.m00() = Sx;
	//cropMatrix.m03() = Ox;

	//cropMatrix.m11() = Sy;
	//cropMatrix.m13() = Oy;

	//return cropMatrix * shadowViewProjMatrix;

}

bool DX11ReflectiveShadowMap::__CreateRenderTargets(void)
{

	Parameters rtParams({ { "renderTarget", "true" } });
	Parameters vsmParams({ { "renderTarget", "true" } });

	Parameters arrayParams = { { "renderTarget", "true" }, { "slices", ToString(m_csmSlices) } };

	m_positionArray.SetParametersList(arrayParams);
	bool ok = m_positionArray.Create(m_resolution, m_resolution, DataFormat::FLOAT4);

	for (int i = 0; i < m_csmSlices; i++)
	{

		m_position.emplace_back(nullptr, "", nullptr, m_device);
		m_flux.emplace_back(nullptr, "", nullptr, m_device);
		m_normal.emplace_back(nullptr, "", nullptr, m_device);

		m_position.back().SetParametersList(rtParams);
		m_flux.back().SetParametersList(rtParams);
		m_normal.back().SetParametersList(rtParams);

		if (m_varianceShadowMapping)
		{
			m_vsm.emplace_back(nullptr, "", nullptr, m_device);
			m_vsm.back().SetParametersList(vsmParams);
		}

		if (!m_position[i].Create(m_resolution, m_resolution, DataFormat::FLOAT4) ||
			!m_normal[i].Create(m_resolution, m_resolution, DataFormat::FLOAT4) ||
			!m_flux[i].Create(m_resolution, m_resolution, DataFormat::FLOAT4) ||
			(m_varianceShadowMapping && !m_vsm[i].Create(m_resolution, m_resolution, DataFormat::FLOAT4)))
		{
			return false;
		}

	}

	return true;

}

void DX11ReflectiveShadowMap::__DestroyRenderTargets(void)
{

	for (int i = 0; i < m_csmSlices; i++)
	{
		m_position[i].Destroy();
		m_flux[i].Destroy();
		m_normal[i].Destroy();
		m_vsm[i].Destroy();
	}

	m_position.clear();
	m_flux.clear();
	m_normal.clear();
	m_vsm.clear();

}

bool DX11ReflectiveShadowMap::__CreateDepthBuffers(void)
{

	for (int i = 0; i < m_csmSlices; i++)
	{

		m_depth.emplace_back(DX11DepthBufferConfiguration { &m_device, m_resolution, m_resolution, true });

		if (!m_depth[i].Create())
		{
			return false;
		}

	}

	return true;

}

void DX11ReflectiveShadowMap::__DestroyDepthBuffers(void)
{

	for (auto & depth : m_depth)
	{
		depth.Destroy();
	}

	m_depth.clear();

}