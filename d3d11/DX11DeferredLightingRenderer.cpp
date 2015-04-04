#include "DX11DeferredLightingRenderer.h"
#include "DX11Module.h"
#include "DX11VertexBuffer.h"
#include "DX11Utils.h"

#include "./detail/ShadersBuffers.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include <mye/math/Math.h>

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

DX11DeferredLightingRenderer::DX11DeferredLightingRenderer(DX11Device & device, Window * window) :
	m_device(device),
	m_window(nullptr),
	m_gbuffer0(nullptr, "", nullptr, device),
	m_gbuffer1(nullptr, "", nullptr, device),
	m_lbuffer(nullptr, "", nullptr, device),
	m_initialized(false),
	m_transformBuffer(nullptr, "", nullptr, device),
	m_lightBuffer(nullptr, "", nullptr, device),
	m_materialBuffer(nullptr, "", nullptr, device),
	m_cameraBuffer(nullptr, "", nullptr, device),
	m_configurationBuffer(nullptr, "", nullptr, device),
	m_clearColor(0.0f),
	m_rsm(device, true)
{

	Parameters params({ { "renderTarget", "true" } });

	m_gbuffer0.SetParametersList(params);
	m_gbuffer1.SetParametersList(params);
	m_lbuffer.SetParametersList(params);

	SetWindow(window);

	DX11DepthBufferConfiguration depthBufferConf;

	depthBufferConf.device         = &device;
	depthBufferConf.height         = 0;
	depthBufferConf.width          = 0;
	depthBufferConf.shaderResource = false;

	m_depthBuffer = DX11DepthBuffer(depthBufferConf);

}


DX11DeferredLightingRenderer::~DX11DeferredLightingRenderer(void)
{

}

bool DX11DeferredLightingRenderer::Init(void)
{

	auto configuration = static_cast<GraphicsModule*>(Game::GetSingleton().GetGraphicsModule())->GetRendererConfiguration();

	m_rsm.SetResolution(configuration->GetShadowMapResolution());

	VertexDeclaration deferredGeometryVD({
		VertexAttribute(VertexAttributeSemantic::POSITION,  DataFormat::FLOAT3),
		VertexAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2),
		VertexAttribute(VertexAttributeSemantic::NORMAL,    DataFormat::FLOAT3)
	});

	VertexDeclaration deferredLightsVD({
		VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT4)
	});

	VertexDeclaration deferredFinalVD({
		VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT3),
		VertexAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2),
		VertexAttribute(VertexAttributeSemantic::NORMAL, DataFormat::FLOAT3)
	});


	auto deferredGeometryILV = MakeInputElementVector(deferredGeometryVD);
	auto deferredLightsILV   = MakeInputElementVector(deferredLightsVD);
	auto deferredFinalILV    = MakeInputElementVector(deferredFinalVD);

	m_deferredGeometryVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/deferred_lighting_geometry_vs.cso",
		nullptr,
		Parameters({
				{ "type", "vertex" },
				{ "precompiled", "true" },
				{ "inputLayoutVector", PointerToString(&deferredGeometryILV) }
		})
	);

	m_deferredLightsVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/deferred_lighting_lights_vs.cso",
		nullptr,
		Parameters({
				{ "type", "vertex" },
				{ "precompiled", "true" },
				{ "inputLayoutVector", PointerToString(&deferredLightsILV) }
		})
	);

	m_deferredFinalVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/deferred_lighting_final_vs.cso",
		nullptr,
		Parameters({
				{ "type", "vertex" },
				{ "precompiled", "true" },
				{ "inputLayoutVector", PointerToString(&deferredFinalILV) }
		})
	);

	m_deferredGeometryPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/deferred_lighting_geometry_ps.cso",
		nullptr,
		Parameters({
				{ "type", "pixel" },
				{ "precompiled", "true" },
		})
	);

	m_deferredLightsPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/deferred_lighting_lights_ps.cso",
		nullptr,
		Parameters({
				{ "type", "pixel" },
				{ "precompiled", "true" },
		})
	);

	m_deferredFinalPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/deferred_lighting_final_ps.cso",
		nullptr,
		Parameters({
				{ "type", "pixel" },
				{ "precompiled", "true" },
		})
	);

	if (m_deferredGeometryVS->Load() &&
		m_deferredGeometryPS->Load() &&
		m_deferredLightsVS->Load() &&
		m_deferredLightsPS->Load() &&
		m_deferredFinalVS->Load() &&
		m_deferredFinalPS->Load() &&
		m_depthBuffer.Create() &&
		m_rsm.Create() &&
		CreateConstantBuffers() &&
		CreateSamplerStates())
	{

		configuration->AddListener(this);

		OnResize(m_window, m_window->GetSize());

		m_window->AddListener(static_cast<IWindow::Listener*>(this));

		m_initialized = true;

	}

	return m_initialized;

}

void DX11DeferredLightingRenderer::Shutdown(void)
{

	if (m_initialized)
	{

		SetWindow(nullptr);

		m_depthBuffer.Destroy();

		m_gbuffer0.Destroy();
		m_gbuffer1.Destroy();
		m_lbuffer.Destroy();

		m_rsm.Destroy();

		m_deferredGeometryVS->Unload();
		m_deferredGeometryPS->Unload();

		m_deferredLightsVS->Unload();
		m_deferredLightsPS->Unload();

		m_deferredFinalVS->Unload();
		m_deferredFinalPS->Unload();

		auto configuration = static_cast<GraphicsModule*>(Game::GetSingleton().GetGraphicsModule())->GetRendererConfiguration();

		configuration->RemoveListener(this);

		DestroyConstantBuffers();
		DestroySamplerStates();

		m_initialized = false;

	}

}

void DX11DeferredLightingRenderer::Render(ID3D11RenderTargetView * target)
{

	SceneModule * scene = Game::GetSingleton().GetSceneModule();
	Camera      * camera = scene->GetCamera();

	if (camera)
	{

		auto visibleObjects = scene->GetVisibleObjects();

		Matrix4 view       = camera->GetViewMatrix();
		Matrix4 projection = camera->GetProjectionMatrix();

		/* First step */

		m_deferredGeometryVS->Use();
		m_deferredGeometryPS->Use();

		ID3D11RenderTargetView * gbuffer[] = {
			m_gbuffer0.GetRenderTargetView(),
			m_gbuffer1.GetRenderTargetView()
		};

		m_gbuffer0.ClearRenderTarget(Vector4f(0.0f));
		m_gbuffer1.ClearRenderTarget(Vector4f(0.0f));

		m_depthBuffer.Clear();

		m_device.GetImmediateContext()->OMSetRenderTargets(
			2,
			gbuffer,
			m_depthBuffer.GetDepthStencilView());

		m_device.SetBlending(false);
		m_device.SetDepthTest(DX11DepthTest::ON);

		for (GameObject * object : visibleObjects)
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
					transformBuffer.worldView           = view * transformBuffer.world;
					transformBuffer.worldViewProjection = projection * transformBuffer.worldView;

					m_transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
					m_transformBuffer.SetData(&transformBuffer);

					MaterialPointer material = rc->GetMaterial();

					detail::MaterialBuffer materialBuffer;

					materialBuffer.diffuseColor  = material->GetDiffuseColor();
					materialBuffer.specularColor = material->GetSpecularColor();
					materialBuffer.specular      = material->GetSpecular();
					materialBuffer.metallic      = material->GetMetallic();
					materialBuffer.roughness     = material->GetRoughness();

					m_materialBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, 0);
					m_materialBuffer.SetData(&materialBuffer);

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

		/* Second step */

		m_lbuffer.ClearRenderTarget(Vector4f(0.0f));

		ID3D11RenderTargetView * lbuffer[] = { m_lbuffer.GetRenderTargetView() };

		Vector4f quad[6] = {
			Vector4f(-1.0f, 1.0f, 0.0f, 1.0f),
			Vector4f(1.0f, -1.0f, 0.0f, 1.0f),
			Vector4f(-1.0f, -1.0f, 0.0f, 1.0f),
			Vector4f(1.0f, 1.0f, 0.0f, 1.0f),
			Vector4f(1.0f, -1.0f, 0.0f, 1.0f),
			Vector4f(-1.0f, 1.0f, 0.0f, 1.0f)
		};

		DX11VertexBuffer quadBuffer(nullptr, "", nullptr, m_device);
		quadBuffer.Create(quad, 6, VertexDeclaration({ VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT4) }));

		DX11ConstantBuffer matrixBuffer(nullptr, "", nullptr, m_device);
		matrixBuffer.Create(sizeof(Matrix4));

		for (Light * light : scene->GetLightsList())
		{

			/* Generate RSM */

			m_rsm.Render(light);

			/* Render to light buffer */

			m_deferredLightsVS->Use();
			m_deferredLightsPS->Use();

			m_device.SetDepthTest(DX11DepthTest::OFF);

			m_device.GetImmediateContext()->OMSetRenderTargets(
				1,
				lbuffer,
				nullptr);


			m_rsm.GetVSMShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

			m_device.GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP, 1, &m_shadowMapSamplerState);
			m_device.GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP, 1, &m_shadowMapSamplerCmpState);

			m_gbuffer0.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER0);
			m_gbuffer1.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER1);

			/* Setup constant buffers */			

			const mye::math::Matrix4 & t = light->GetWorldMatrix();

			detail::LightBuffer lightBuffer;
			
			lightBuffer.color     = mye::math::Vector4f(light->GetColor() * light->GetIntensity(), 1);
			lightBuffer.position  = t * mye::math::Vector4f(light->GetPosition(), 1);
			lightBuffer.direction = t * mye::math::Vector4f(light->GetDirection(), 1);
			lightBuffer.spotAngle = light->GetSpotAngle();
			lightBuffer.range     = light->GetRange();

			m_lightBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);
			m_lightBuffer.SetData(&lightBuffer);

			detail::CameraBuffer cameraBuffer;

			cameraBuffer.position = mye::math::Vector4f(camera->GetPosition(), 1);

			m_cameraBuffer.SetData(&cameraBuffer);
			m_cameraBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, 1);

			matrixBuffer.SetData(&m_rsm.GetLightSpaceTransformMatrix());
			matrixBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM);

			m_configurationBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_RENDERCONFIGURATION);

			/* Render the quad */

			quadBuffer.Bind();

			m_device.GetImmediateContext()->
				IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_device.GetImmediateContext()->
				Draw(quadBuffer.GetVerticesCount(), 0);

			quadBuffer.Unbind();

			m_rsm.GetVSMShaderResource().Unbind();

		}

		matrixBuffer.Destroy();

		quadBuffer.Destroy();

		m_gbuffer0.Unbind();
		m_gbuffer1.Unbind();

		/* Final pass */

		m_deferredFinalVS->Use();
		m_deferredFinalPS->Use();

		m_device.SetDepthTest(DX11DepthTest::LOOKUP);

		m_device.GetImmediateContext()->ClearRenderTargetView(target, m_clearColor.Data());

		m_device.GetImmediateContext()->OMSetRenderTargets(
			1,
			&target,
			m_depthBuffer.GetDepthStencilView());

		m_lbuffer.Bind(DX11PipelineStage::PIXEL_SHADER, 0);

		for (GameObject * object : visibleObjects)
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
					transformBuffer.worldView           = view * transformBuffer.world;
					transformBuffer.worldViewProjection = projection * transformBuffer.worldView;

					m_transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
					m_transformBuffer.SetData(&transformBuffer);

					MaterialPointer material = rc->GetMaterial();

					detail::MaterialBuffer materialBuffer;

					materialBuffer.diffuseColor  = material->GetDiffuseColor();
					materialBuffer.specularColor = material->GetSpecularColor();
					materialBuffer.specular      = material->GetSpecular();
					materialBuffer.metallic      = material->GetMetallic();
					materialBuffer.roughness     = material->GetRoughness();

					m_materialBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_MATERIAL);
					m_materialBuffer.SetData(&materialBuffer);

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

	}

}

void DX11DeferredLightingRenderer::SetWindow(Window * window)
{

	if (m_initialized)
	{

		if (m_window)
		{
			m_window->RemoveListener(this);
		}

		m_window = window;

		if (window)
		{

			OnResize(m_window, m_window->GetSize());

			m_window->AddListener(this);

		}

	}
	else
	{
		m_window = window;

	}


}

void DX11DeferredLightingRenderer::OnResize(IWindow * window, const Vector2i & size)
{

	m_gbuffer0.Destroy();
	m_gbuffer1.Destroy();
	m_lbuffer.Destroy();

	m_gbuffer0.Create(size.x(), size.y(), DataFormat::FLOAT4);
	m_gbuffer1.Create(size.x(), size.y(), DataFormat::FLOAT4);
	m_lbuffer.Create(size.x(), size.y(), DataFormat::FLOAT4);

	m_depthBuffer.Resize(size.x(), size.y());

	static_cast<GraphicsModule*>(Game::GetSingleton().GetGraphicsModule())->GetRendererConfiguration()->SetScreenResolution(size);

}

bool DX11DeferredLightingRenderer::CreateConstantBuffers(void)
{

	return
		m_transformBuffer.Create(sizeof(detail::TransformBuffer)) &&
		m_lightBuffer.Create(sizeof(detail::LightBuffer)) &&
		m_cameraBuffer.Create(sizeof(detail::CameraBuffer)) &&
		m_materialBuffer.Create(sizeof(detail::MaterialBuffer)) &&
		m_configurationBuffer.Create(sizeof(detail::RenderConfigurationBuffer));

}

void DX11DeferredLightingRenderer::DestroyConstantBuffers(void)
{
	m_transformBuffer.Destroy();
	m_lightBuffer.Destroy();
	m_cameraBuffer.Destroy();
	m_materialBuffer.Destroy();
}

bool DX11DeferredLightingRenderer::CreateSamplerStates(void)
{

	D3D11_SAMPLER_DESC shadowMapSamplerDesc;

	shadowMapSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//shadowMapSamplerDesc.Filter         = D3D11_FILTER_ANISOTROPIC;
	shadowMapSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	shadowMapSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	shadowMapSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	shadowMapSamplerDesc.MipLODBias     = 0.0f;
	shadowMapSamplerDesc.MaxAnisotropy  = 16;
	shadowMapSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	shadowMapSamplerDesc.BorderColor[0] = 0;
	shadowMapSamplerDesc.BorderColor[1] = 0;
	shadowMapSamplerDesc.BorderColor[2] = 0;
	shadowMapSamplerDesc.BorderColor[3] = 0;
	shadowMapSamplerDesc.MinLOD         = 0;
	shadowMapSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	D3D11_SAMPLER_DESC shadowMapSamplerCmpDesc;

	shadowMapSamplerCmpDesc.Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowMapSamplerCmpDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerCmpDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerCmpDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerCmpDesc.MipLODBias     = 0.0f;
	shadowMapSamplerCmpDesc.MaxAnisotropy  = 1;
	shadowMapSamplerCmpDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowMapSamplerCmpDesc.BorderColor[0] = 0;
	shadowMapSamplerCmpDesc.BorderColor[1] = 0;
	shadowMapSamplerCmpDesc.BorderColor[2] = 0;
	shadowMapSamplerCmpDesc.BorderColor[3] = 0;
	shadowMapSamplerCmpDesc.MinLOD         = 0;
	shadowMapSamplerCmpDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	return !__MYE_DX11_HR_TEST_FAILED(m_device->CreateSamplerState(&shadowMapSamplerDesc,    &m_shadowMapSamplerState)) &&
	       !__MYE_DX11_HR_TEST_FAILED(m_device->CreateSamplerState(&shadowMapSamplerCmpDesc, &m_shadowMapSamplerCmpState));

}

void DX11DeferredLightingRenderer::DestroySamplerStates(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shadowMapSamplerCmpState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shadowMapSamplerState);
}

void DX11DeferredLightingRenderer::OnConfigurationChange(RendererVariable variable, RendererConfiguration * configuration)
{

	switch (variable)
	{

	case RendererVariable::SHADOWMAPRESOLUTION:
		m_rsm.SetResolution(configuration->GetShadowMapResolution());
		break;

	case RendererVariable::RESOLUTION:
		// Resize window
		break;

	}

	detail::RenderConfigurationBuffer r;

	r.resolution          = configuration->GetScreenResolution();
	r.gamma               = configuration->GetGamma();

	r.shadowMapResolution = configuration->GetShadowMapResolution();

	r.vsmMinVariance      = configuration->GetVSMMinVariance();
	r.vsmMinBleeding      = configuration->GetVSMMinBleeding();

	m_configurationBuffer.SetData(&r);

}