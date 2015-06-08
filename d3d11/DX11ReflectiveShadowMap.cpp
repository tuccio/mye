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

#define __MYE_RSM_DEFAULT_RESOLUTION 2048

DX11ReflectiveShadowMap::DX11ReflectiveShadowMap(DX11Device & device, bool vsm) :
	m_position(nullptr, "", nullptr, device),
	m_normal(nullptr, "", nullptr, device),
	m_flux(nullptr, "", nullptr, device),
	m_vsm(nullptr, "", nullptr, device),
	m_depth({ &device, 0, 0, true }),
	m_device(device),
	m_varianceShadowMapping(vsm),
	m_initialized(false)
{
	SetResolution(__MYE_RSM_DEFAULT_RESOLUTION);
}

bool DX11ReflectiveShadowMap::Create(void)
{

	Parameters rtParams({ { "renderTarget", "true" } });
	Parameters vsmParams({ { "renderTarget", "true"} });
	rtParams = vsmParams;

	m_position.SetParametersList(rtParams);
	m_normal.SetParametersList(rtParams);
	m_flux.SetParametersList(rtParams);
	m_vsm.SetParametersList(vsmParams);

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
		CreateRenderTargets() &&
		m_depth.Create())
	{
		m_initialized = true;
	}

	return m_initialized;

}
void DX11ReflectiveShadowMap::Destroy(void)
{

	m_position.Destroy();
	m_normal.Destroy();
	m_flux.Destroy();
	m_depth.Destroy();
	m_vsm.Destroy();

	m_initialized = false;

}

void DX11ReflectiveShadowMap::SetResolution(int resolution)
{
	m_resolution = resolution;

	if (m_initialized)
	{

		m_position.Destroy();
		m_normal.Destroy();
		m_flux.Destroy();
		m_vsm.Destroy();

		CreateRenderTargets();

	}

	m_depth.Resize(resolution, resolution);
	
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
		RenderDirectional(light);
		break;

	}

	m_device.GetImmediateContext()->RSSetViewports(numViewports, oldViewports);

}

void DX11ReflectiveShadowMap::Bind(DX11PipelineStage stage)
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

void DX11ReflectiveShadowMap::RenderDirectional(Light * light)
{

	auto scene = Game::GetSingleton().GetSceneModule();

	Camera * camera = scene->GetCamera();

	auto viewFrustum = camera->GetFrustum();

	auto viewFrustumCorners = viewFrustum.GetCorners();
	Vector3 centroid = Vector3(0);

	for (auto & corner : viewFrustumCorners)
	{
		centroid += corner;
	}

	centroid = centroid / 8;

	Vector3 lightViewPosition = centroid;
	Vector3 lightDirection    = light->GetDirection();
	Real    lightRange        = light->GetRange();

	Camera lightCamera;


	lightCamera.LookAt(lightViewPosition, Vector3(0, 1, 0), lightViewPosition + lightDirection);

	Matrix4 lightView = lightCamera.GetViewMatrix();

	std::vector<Vector3> lightFrustumCorners;

	for (auto & corner : viewFrustumCorners)
	{
		lightFrustumCorners.push_back(lightView * corner);
	}

	auto minMaxX = std::minmax_element(lightFrustumCorners.begin(),
									   lightFrustumCorners.end(),
									   [] (const Vector3 & a, const Vector3 & b) { return a.x() < b.x(); });

	auto minMaxY = std::minmax_element(lightFrustumCorners.begin(),
									   lightFrustumCorners.end(),
									   [] (const Vector3 & a, const Vector3 & b) { return a.y() < b.y(); });

	auto minMaxZ = std::minmax_element(lightFrustumCorners.begin(),
									   lightFrustumCorners.end(),
									   [] (const Vector3 & a, const Vector3 & b) { return a.z() < b.z(); });

	Real l = - lightRange * minMaxX.second->x();
	Real r = - lightRange * minMaxX.first->x();
	Real b = lightRange * minMaxY.first->y();
	Real t = lightRange * minMaxY.second->y();
	Real n = lightRange * minMaxZ.first->z();
	Real f = lightRange * minMaxZ.second->z();
	
	Matrix4 lightProjection = OrthographicProjection(l, r, b, t, n, f);
	
	m_lightSpaceTransform = lightProjection * lightView;

	auto objects = scene->GetVisibleObjects(m_lightSpaceTransform);

	ID3D11RenderTargetView * renderTargets[] =
	{
		m_position.GetRenderTargetView(),
		m_normal.GetRenderTargetView(),
		m_flux.GetRenderTargetView(),
		m_vsm.GetRenderTargetView()
	};

	int nRenderTargets;

	m_rsmVS->Use();

	if (m_varianceShadowMapping)
	{
		m_rsmVSMPS->Use();
		m_vsm.ClearRenderTarget(Vector4(0));
		nRenderTargets = 4;
	}
	else
	{
		m_rsmPS->Use();
		nRenderTargets = 3;
	}

	m_position.ClearRenderTarget(Vector4(0));
	m_normal.ClearRenderTarget(Vector4(0));
	m_flux.ClearRenderTarget(Vector4(0));

	m_depth.Clear();

	m_device.SetDepthTest(DX11DepthTest::ON);
	m_device.GetImmediateContext()->OMSetRenderTargets(nRenderTargets,
													   renderTargets,
													   m_depth.GetDepthStencilView());

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
				transformBuffer.worldView           = transformBuffer.world;
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
		m_vsm.GenerateMips();
	}

	transformCBuffer.Destroy();
	materialCBuffer.Destroy();
	lightCBuffer.Destroy();

}

bool DX11ReflectiveShadowMap::CreateRenderTargets(void)
{
	return m_position.Create(m_resolution, m_resolution, DataFormat::FLOAT4) &&
		m_normal.Create(m_resolution, m_resolution, DataFormat::FLOAT4) &&
		m_flux.Create(m_resolution, m_resolution, DataFormat::FLOAT4) &&
		(!m_varianceShadowMapping || m_vsm.Create(m_resolution, m_resolution, DataFormat::FLOAT2));
}