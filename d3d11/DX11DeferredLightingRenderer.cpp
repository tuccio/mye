#include "DX11DeferredLightingRenderer.h"
#include "DX11Module.h"
#include "DX11RasterizerState.h"
#include "DX11Utils.h"
#include "DX11VertexBuffer.h"
#include "DX11LightPropagationVolume.h"

#include "./detail/ShadersBuffers.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include <mye/math/Math.h>

#include <random>

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

DX11DeferredLightingRenderer::DX11DeferredLightingRenderer(void) :
	m_initialized(false),
	m_clearColor(0.0f),
	m_renderConfigurationUptodate(false)
{

	Parameters params({ { "renderTarget", "true" } });

	m_gbuffer0.SetParametersList(params);
	m_gbuffer1.SetParametersList(params);
	m_lbuffer.SetParametersList(params);

	DX11DepthBufferConfiguration depthBufferConf;

	depthBufferConf.height         = 0;
	depthBufferConf.width          = 0;
	depthBufferConf.shaderResource = false;
	depthBufferConf.arraySize      = 1;

	m_depthBuffer = DX11DepthBuffer(depthBufferConf);

}


DX11DeferredLightingRenderer::~DX11DeferredLightingRenderer(void)
{

}

bool DX11DeferredLightingRenderer::Init(void)
{

	auto configuration = static_cast<GraphicsModule*>(Game::GetSingleton().GetGraphicsModule())->GetRendererConfiguration();

	m_rsm.SetResolution(configuration->GetShadowMapResolution());
	m_rsm.SetCSMSplits(configuration->GetCSMSplits());

	VertexDeclaration deferredGeometryVD({
		VertexAttribute(VertexAttributeSemantic::POSITION,  DataFormat::FLOAT3),
		VertexAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2),
		VertexAttribute(VertexAttributeSemantic::NORMAL,    DataFormat::FLOAT3)
	});

	VertexDeclaration deferredLightsVD({
		VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT4)
	});

	VertexDeclaration deferredFinalVD({
		VertexAttribute(VertexAttributeSemantic::POSITION,  DataFormat::FLOAT3),
		VertexAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2),
		VertexAttribute(VertexAttributeSemantic::NORMAL,    DataFormat::FLOAT3)
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

	m_deferredLPVPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/deferred_lighting_lpv_ps.cso",
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
		m_deferredLPVPS->Load() &&
		m_deferredFinalVS->Load() &&
		m_deferredFinalPS->Load() &&
		m_depthBuffer.Create() &&
		m_rsm.Create() &&
		m_lpv.Create(configuration->GetLPVResolution(), configuration->GetLPVRSMSamples()) &&
		__CreateConstantBuffers() &&
		__CreateContextStates())
	{

		static ManualLambdaLoader cosSinLoader(std::function<bool(Resource*)>(
		[] (Resource * r)
		{
			
			DX11Texture * t = static_cast<DX11Texture*>(r);
			
			//Vector2i resolution = DX11Module::GetSingleton().GetRendererConfiguration()->GetScreenResolution();

			const unsigned int resolution = 256;

			struct __CosSin
			{
				float cosAlpha;
				float sinAlpha;
			};

			std::vector<__CosSin> data(resolution * resolution);
			
			std::random_device                    device;
			std::mt19937                          generator(device());
			std::uniform_real_distribution<float> distribution(0.f, TwoPi<float>());

			for (auto & e : data)
			{
				float alpha = distribution(generator);
				e.cosAlpha  = Cosine(alpha);
				e.sinAlpha  = Sine(alpha);
			}

			return t->Create(resolution, resolution, DataFormat::FLOAT2, &data[0]);

		}),

		std::function<void(Resource*)>(
			[] (Resource * r)
		{
			DX11Texture * t = static_cast<DX11Texture*>(r);
			t->Destroy();
		}));

		m_randomCosSin = ResourceTypeManager::GetSingleton().CreateResource<DX11Texture>("Texture",
		                                                                                 "MYE_RANDOM_COS_SIN",
		                                                                                 &cosSinLoader);

		MYE_EVENT_MANAGER_ADD_LISTENER(this, EventType::RENDERER_VARIABLE_CHANGE);

		__ResizeBuffers(DX11Module::GetSingleton().GetRendererConfiguration()->GetScreenResolution());

		m_initialized = true;

	}

	return m_initialized;

}

void DX11DeferredLightingRenderer::Shutdown(void)
{

	if (m_initialized)
	{

		m_depthBuffer.Destroy();

		m_gbuffer0.Destroy();
		m_gbuffer1.Destroy();
		m_lbuffer.Destroy();

		m_rsm.Destroy();
		m_lpv.Destroy();

		m_deferredGeometryVS->Unload();
		m_deferredGeometryPS->Unload();

		m_deferredLightsVS->Unload();
		m_deferredLightsPS->Unload();

		m_deferredFinalVS->Unload();
		m_deferredFinalPS->Unload();

		auto configuration = static_cast<GraphicsModule*>(Game::GetSingleton().GetGraphicsModule())->GetRendererConfiguration();

		MYE_EVENT_MANAGER_REMOVE_LISTENER(this, EventType::RENDERER_VARIABLE_CHANGE);

		__DestroyConstantBuffers();
		__DestroyContextStates();

		m_initialized = false;

	}

}

void DX11DeferredLightingRenderer::Render(ID3D11RenderTargetView * target)
{

	__UpdateRenderConfiguration();

	SceneModule * scene   = Game::GetSingleton().GetSceneModule();
	Camera      * camera  = scene->GetCamera();

	if (camera)
	{

		Matrix4 view           = camera->GetViewMatrix();
		Matrix4 projection     = camera->GetProjectionMatrix();
		Matrix4 viewProjection = projection * view;

		auto visibleObjects = scene->GetVisibleObjects(*camera);
		//auto visibleObjects = scene->GetObjectsList();

		DX11RasterizerState backCull({ false, CullMode::BACK });

		auto corners = camera->GetFrustum().GetCorners();

		/* First step (geometry buffer) */

		m_deferredGeometryVS->Use();
		m_deferredGeometryPS->Use();

		ID3D11RenderTargetView * gbuffer[] = {
			m_gbuffer0.GetRenderTargetView(),
			m_gbuffer1.GetRenderTargetView()
		};

		m_gbuffer0.ClearRenderTarget(Vector4f(0.f));
		m_gbuffer1.ClearRenderTarget(Vector4f(0.f));

		m_depthBuffer.Clear();

		DX11Device::GetSingleton().GetImmediateContext()->
			OMSetRenderTargets(2,
			                   gbuffer,
			                   m_depthBuffer.GetDepthStencilView());

		DX11Device::GetSingleton().SetBlending(false);
		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::ON);

		backCull.Use();

		for (GameObject * object : visibleObjects)
		{

			RenderComponent * rc = object->GetRenderComponent();

			if (rc)
			{

				TransformComponent * tc = object->GetTransformComponent();

				GPUBufferPointer gpuBuffer = rc->GetGPUBuffer();

				if (gpuBuffer && gpuBuffer->Load())
				{

					MakeTransformBuffer(m_transformBuffer, tc->GetWorldMatrix() * rc->GetModelMatrix(), view, projection);
					MakeMaterialBuffer(m_materialBuffer, rc->GetMaterial());

					m_transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
					m_materialBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_MATERIAL);

					DX11VertexBufferPointer vertexBuffer = Resource::StaticCast<DX11VertexBuffer>(gpuBuffer);
					vertexBuffer->Bind();

					DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					DX11Device::GetSingleton().GetImmediateContext()->Draw(vertexBuffer->GetVerticesCount(), 0);

					vertexBuffer->Unbind();

				}

			}

		}

		/* Second step (light buffer) */

		m_lbuffer.ClearRenderTarget(Vector4(.0f));

		ID3D11RenderTargetView * lbuffer = m_lbuffer.GetRenderTargetView();

		float quad[] = {
			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		DX11VertexBuffer quadBuffer;
		quadBuffer.Create(quad, 6, VertexDeclaration({ VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT4) }));

		DX11ConstantBuffer matrixBuffer;
		matrixBuffer.Create(sizeof(Matrix4));

		MakeConstantBuffer(m_cameraTransformBuffer, view, viewProjection, viewProjection.Inverse());

		DX11ConstantBuffer pssmSliceBuffer;
		pssmSliceBuffer.Create(sizeof(detail::PSSMSlice) * m_rsm.GetCSMSplits());

		m_cameraTransformBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM);

		m_randomCosSin->Load();

		//AABB lpvVolume = BoundingAABB(camera->GetFrustum());
		AABB lpvVolume = AABB::FromCenterHalfExtents(Vector3(0.f), Vector3(10.f));

		m_lpv.Init(camera, lpvVolume.GetMinimum(), lpvVolume.GetMaximum());

		for (Light * light : scene->GetLightsList())
		{


			/* Generate RSM */

			m_rsm.Render(light);

			int slices;

			if (light->GetType() == LightType::DIRECTIONAL)
			{
				
				slices = m_rsm.GetCSMSplits();

				std::vector<detail::PSSMSlice> pssmBufferData;

				auto & sliceSplits = m_rsm.GetPSSMSlices();

				for (int i = 0; i < slices; i++)
				{
					pssmBufferData.push_back({ m_rsm.GetPSSMCropMatrix(i), sliceSplits[i].near, sliceSplits[i].far });
				}

				pssmSliceBuffer.SetData(&pssmBufferData[0]);
				pssmSliceBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_PSSMBUFFER);

				for (int i = 0; i < slices; i++)
				{
					int debugTexSize = 150;
					DX11Module::GetSingleton().RenderShaderResource(m_rsm.GetDepthShaderResource(), Vector2i(i * debugTexSize, 0), Vector2i(debugTexSize), i);
					//DX11Module::GetSingleton().RenderShaderResource(m_rsm.GetPositionShaderResource(), Vector2i(i * debugTexSize, 0), Vector2i(debugTexSize), i);
				}
				


			}
			else
			{
				slices = 1;
			}

			/* Render to light buffer */

			m_deferredLightsVS->Use();
			m_deferredLightsPS->Use();

			DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

			DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &lbuffer, nullptr);

			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP,     1, &m_shadowMapSamplerState);
			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP, 1, &m_shadowMapSamplerCmpState);
			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_RANDOM,        1, &m_randomSamplerState);

			m_gbuffer0.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER0);
			m_gbuffer1.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER1);

			m_randomCosSin->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_RANDOM);

			/* Setup constant buffers */

			MakeLightBuffer(m_lightBuffer, light);
			m_lightBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

			DX11Device::GetSingleton().SetBlending(true);

			const Matrix4      & shadowViewProjMatrix = m_rsm.GetLightSpaceTransformMatrix();
			DX11ShaderResource & rsmDepth             = m_rsm.GetDepthShaderResource();

			rsmDepth.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

			matrixBuffer.SetData(&shadowViewProjMatrix);
			matrixBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM);

			m_configurationBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_RENDERCONFIGURATION);

			/* Render the quad */

			DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_accumulateBlendState, nullptr, 0xFFFFFFFF);

			backCull.Use();

			quadBuffer.Bind();

			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DX11Device::GetSingleton().GetImmediateContext()->Draw(quadBuffer.GetVerticesCount(), 0);

			quadBuffer.Unbind();

			rsmDepth.Unbind();

			m_gbuffer0.Unbind();
			m_gbuffer1.Unbind();

			m_randomCosSin->Unbind();

			/* Inject LPV */

			m_cameraTransformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, __MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM);

			m_lpv.Inject(m_rsm);

		}

		m_lpv.Propagate(DX11Module::GetSingleton().GetRendererConfiguration()->GetLPVIterations());

		/* Render LPV */

		/* TODO: SETUP THE SHADERS TO RENDER LPV TO LIGHTBUFFER AND THEIR CONSTANT BUFFERS */

		//m_lbuffer.ClearRenderTarget(Vector4(0));

		m_deferredLightsVS->Use();
		m_deferredLPVPS->Use();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &lbuffer, nullptr);

		m_lpv.BindConfigurationBuffer(DX11PipelineStage::PIXEL_SHADER, 0);

		DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_accumulateBlendState, nullptr, 0xFFFFFFFF);
		DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_LPV, 1, &m_lpvSamplerState);
		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

		m_gbuffer0.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER0);
		m_gbuffer1.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER1);

		m_lpv.GetLightVolume().Bind(DX11PipelineStage::PIXEL_SHADER,
		                            __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED,
		                            __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN,
		                            __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);

		quadBuffer.Bind();

		DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11Device::GetSingleton().GetImmediateContext()->Draw(quadBuffer.GetVerticesCount(), 0);

		quadBuffer.Unbind();

		m_gbuffer0.Unbind();
		m_gbuffer1.Unbind();

		m_lpv.GetLightVolume().Unbind();

		matrixBuffer.Destroy();
		quadBuffer.Destroy();

		/* Final pass (shading) */

		m_deferredFinalVS->Use();
		m_deferredFinalPS->Use();

		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::LOOKUP);

		DX11Device::GetSingleton().GetImmediateContext()->ClearRenderTargetView(target, m_clearColor.Data());
		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &target, m_depthBuffer.GetDepthStencilView());

		m_lbuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_LIGHTBUFFER);

		backCull.Use();

		for (GameObject * object : visibleObjects)
		{

			RenderComponent * rc = object->GetRenderComponent();

			if (rc)
			{

				TransformComponent * tc = object->GetTransformComponent();

				GPUBufferPointer gpuBuffer = rc->GetGPUBuffer();

				if (gpuBuffer && gpuBuffer->Load())
				{

					MakeTransformBuffer(m_transformBuffer, tc->GetWorldMatrix() * rc->GetModelMatrix(), view, projection);
					MakeMaterialBuffer(m_materialBuffer, rc->GetMaterial());

					m_transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
					m_materialBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_MATERIAL);

					DX11VertexBufferPointer vertexBuffer = Resource::StaticCast<DX11VertexBuffer>(gpuBuffer);
					vertexBuffer->Bind();

					DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					DX11Device::GetSingleton().GetImmediateContext()->Draw(vertexBuffer->GetVerticesCount(), 0);

					vertexBuffer->Unbind();

				}

			}

		}

		m_lbuffer.Unbind();

		DX11Module::GetSingleton().RenderShaderResource(m_lbuffer, Vector2i(300, 0), Vector2i(267, 150));

		pssmSliceBuffer.Destroy();

	}

}

bool DX11DeferredLightingRenderer::__CreateConstantBuffers(void)
{

	return m_transformBuffer.Create(sizeof(detail::TransformBuffer)) &&
	       m_lightBuffer.Create(sizeof(detail::LightBuffer)) &&
		   m_cameraTransformBuffer.Create(sizeof(Matrix4) * 3) &&
	       m_materialBuffer.Create(sizeof(detail::MaterialBuffer)) &&
	       m_configurationBuffer.Create(sizeof(detail::RenderConfigurationBuffer));

}

void DX11DeferredLightingRenderer::__DestroyConstantBuffers(void)
{
	m_transformBuffer.Destroy();
	m_lightBuffer.Destroy();
	m_cameraTransformBuffer.Destroy();
	m_materialBuffer.Destroy();
}

bool DX11DeferredLightingRenderer::__CreateContextStates(void)
{

	D3D11_SAMPLER_DESC shadowMapSamplerDesc;

	shadowMapSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//shadowMapSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
	shadowMapSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerDesc.MipLODBias     = 0.0f;
	shadowMapSamplerDesc.MaxAnisotropy  = 0;
	shadowMapSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	shadowMapSamplerDesc.BorderColor[0] = 0;
	shadowMapSamplerDesc.BorderColor[1] = 0;
	shadowMapSamplerDesc.BorderColor[2] = 0;
	shadowMapSamplerDesc.BorderColor[3] = 0;
	shadowMapSamplerDesc.MinLOD         = 0;
	shadowMapSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;	

	D3D11_SAMPLER_DESC shadowMapSamplerCmpDesc;

	shadowMapSamplerCmpDesc.Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	//shadowMapSamplerCmpDesc.Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	shadowMapSamplerCmpDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerCmpDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerCmpDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowMapSamplerCmpDesc.MipLODBias     = 0.0f;
	shadowMapSamplerCmpDesc.MaxAnisotropy  = 0;
	shadowMapSamplerCmpDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowMapSamplerCmpDesc.BorderColor[0] = 0;
	shadowMapSamplerCmpDesc.BorderColor[1] = 0;
	shadowMapSamplerCmpDesc.BorderColor[2] = 0;
	shadowMapSamplerCmpDesc.BorderColor[3] = 0;
	shadowMapSamplerCmpDesc.MinLOD         = 0;
	shadowMapSamplerCmpDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	D3D11_SAMPLER_DESC lpvSamplerDesc;

	lpvSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	lpvSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
	lpvSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
	lpvSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
	lpvSamplerDesc.MipLODBias     = 0.0f;
	lpvSamplerDesc.MaxAnisotropy  = 0;
	lpvSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	lpvSamplerDesc.BorderColor[0] = 0;
	lpvSamplerDesc.BorderColor[1] = 0;
	lpvSamplerDesc.BorderColor[2] = 0;
	lpvSamplerDesc.BorderColor[3] = 0;
	lpvSamplerDesc.MinLOD         = 0;
	lpvSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	D3D11_SAMPLER_DESC randomSamplerDesc;

	randomSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
	randomSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	randomSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	randomSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	randomSamplerDesc.MipLODBias     = 0.0f;
	randomSamplerDesc.MaxAnisotropy  = 0;
	randomSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	randomSamplerDesc.BorderColor[0] = 0;
	randomSamplerDesc.BorderColor[1] = 0;
	randomSamplerDesc.BorderColor[2] = 0;
	randomSamplerDesc.BorderColor[3] = 0;
	randomSamplerDesc.MinLOD         = 0;
	randomSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	D3D11_BLEND_DESC accumulateBlendStateDesc;
	ZeroMemory(&accumulateBlendStateDesc, sizeof(D3D11_BLEND_DESC));

	accumulateBlendStateDesc.RenderTarget[0].BlendEnable           = TRUE;
	accumulateBlendStateDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
	accumulateBlendStateDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
	accumulateBlendStateDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	accumulateBlendStateDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	accumulateBlendStateDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ONE;
	accumulateBlendStateDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	accumulateBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&shadowMapSamplerDesc,    &m_shadowMapSamplerState)) &&
	       !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&shadowMapSamplerCmpDesc, &m_shadowMapSamplerCmpState)) &&
		   !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&lpvSamplerDesc,          &m_lpvSamplerState)) &&
		   !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&randomSamplerDesc,       &m_randomSamplerState)) &&
	       !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateBlendState(&accumulateBlendStateDesc, &m_accumulateBlendState));

}

void DX11DeferredLightingRenderer::__DestroyContextStates(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shadowMapSamplerCmpState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shadowMapSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_lpvSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_randomSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shadowMapSamplerState);
}

void DX11DeferredLightingRenderer::OnEvent(const IEvent * event)
{

	const RenderVariableChange * rEvent = static_cast<const RenderVariableChange *>(event);

	switch (rEvent->variable)
	{

	case RendererVariable::SHADOWMAPRESOLUTION:
		m_rsm.SetResolution(rEvent->configuration->GetShadowMapResolution());
		break;

	case RendererVariable::CSMSPLITS:
		m_rsm.SetCSMSplits(rEvent->configuration->GetCSMSplits());
		break;

	case RendererVariable::RESOLUTION:
		
		/* Resize buffers */

		if (m_initialized)
		{

			__ResizeBuffers(rEvent->configuration->GetScreenResolution());

		}

		m_randomCosSin->Unload();

		break;

	}

	m_renderConfigurationUptodate.store(false);

}

void DX11DeferredLightingRenderer::__UpdateRenderConfiguration(void)
{

	bool bFalse = false;

	if (m_renderConfigurationUptodate.compare_exchange_strong(bFalse, true))
	{

		auto configuration = DX11Module::GetSingleton().GetRendererConfiguration();

		detail::RenderConfigurationBuffer r;

		r.resolution[0]             = configuration->GetScreenResolution().x();
		r.resolution[1]             = configuration->GetScreenResolution().y();
		r.gamma                     = configuration->GetGamma();

		r.shadowMapResolution       = configuration->GetShadowMapResolution();
		r.shadowMapInvResolution    = 1.f / r.shadowMapResolution;
		r.shadowMapBias             = configuration->GetShadowMapBias();
		r.shadowMapSlopeScaledBias  = configuration->GetShadowMapSlopeScaledBias();
		r.shadowMapNormalOffsetBias = configuration->GetShadowMapNormalOffsetBias();

		r.vsmMinVariance            = configuration->GetVSMMinVariance();
		r.vsmMinBleeding            = configuration->GetVSMMinBleeding();

		r.csmSplits                 = configuration->GetCSMSplits();
		r.csmDebug                  = configuration->GetCSMDebug();

		r.pcfEnabled                = configuration->GetPCFEnabled();
		r.pcfKernel                 = configuration->GetPCFKernel();
		r.pcfKernelInvSquare        = 1.f / (r.pcfKernel * r.pcfKernel);

		m_configurationBuffer.SetData(&r);

		m_renderConfigurationUptodate.store(true);

	}

}

void DX11DeferredLightingRenderer::__ResizeBuffers(const Vector2i & size)
{

	m_gbuffer0.Destroy();
	m_gbuffer1.Destroy();
	m_lbuffer.Destroy();

	m_gbuffer0.Create(size.x(), size.y(), DataFormat::FLOAT4);
	m_gbuffer1.Create(size.x(), size.y(), DataFormat::FLOAT4);
	m_lbuffer.Create(size.x(), size.y(), DataFormat::FLOAT4);

	m_depthBuffer.Resize(size.x(), size.y());

}