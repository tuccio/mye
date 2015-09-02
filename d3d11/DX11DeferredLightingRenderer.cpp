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

#include <mye/core/Logger.h>

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

	m_deferredGeometry[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                              "./shaders/deferred_lighting_geometry.msh",
	                                                                                              nullptr,
	                                                                                              { { "type", "program" } });

	m_deferredGeometry[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                              "./shaders/deferred_lighting_geometry_heightmap.msh",
	                                                                                              nullptr,
	                                                                                              { { "type", "program" } });

	m_deferredLights = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                         "./shaders/deferred_lighting_lights.msh",
	                                                                                         nullptr,
	                                                                                         { { "type", "program" } });

	m_deferredLightsLPV = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                            "./shaders/deferred_lighting_lights_lpv.msh",
	                                                                                            nullptr,
	                                                                                            { { "type", "program" } });

	m_deferredFinal[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                           "./shaders/deferred_lighting_final.msh",
	                                                                                           nullptr,
	                                                                                           { { "type", "program" } });

	m_deferredFinal[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                           "./shaders/deferred_lighting_final_difftex.msh",
	                                                                                           nullptr,
	                                                                                           { { "type", "program" } });

	auto configuration = static_cast<GraphicsModule *>(Game::GetSingleton().GetGraphicsModule())->GetRendererConfiguration();

	m_rsm.SetResolution(configuration->GetShadowMapResolution());
	m_rsm.SetCSMSplits(configuration->GetCSMSplits());

	if (m_deferredGeometry[0]->Load() &&
		m_deferredGeometry[1]->Load() &&
	    m_deferredLights->Load() &&
	    m_deferredLightsLPV->Load() &&
	    m_deferredFinal[0]->Load() &&
	    m_deferredFinal[1]->Load() &&
	    m_depthBuffer.Create() &&
	    m_rsm.Create() &&
	    m_lpv.Create(configuration->GetLPVResolution(), configuration->GetLPVRSMSamples()) &&
	    __CreateConstantBuffers() &&
	    __CreateContextStates())
	{

		m_randomCosSin     = ResourceTypeManager::GetSingleton().GetResource<DX11Texture>("Texture", "MYE_RANDOM_COS_SIN");
		m_quadVertexBuffer = ResourceTypeManager::GetSingleton().GetResource<DX11VertexBuffer>("GPUBuffer", "MYE_QUAD");

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

		m_deferredGeometry[0] = nullptr;
		m_deferredGeometry[1] = nullptr;

		m_deferredLights      = nullptr;
		m_deferredLightsLPV   = nullptr;

		m_deferredFinal[0]    = nullptr;
		m_deferredFinal[1]    = nullptr;

		auto configuration = static_cast<GraphicsModule *>(Game::GetSingleton().GetGraphicsModule())->GetRendererConfiguration();

		MYE_EVENT_MANAGER_REMOVE_LISTENER(this, EventType::RENDERER_VARIABLE_CHANGE);

		__DestroyConstantBuffers();
		__DestroyContextStates();

		m_initialized = false;

	}

}

void DX11DeferredLightingRenderer::Render(ID3D11RenderTargetView * target)
{

	__UpdateRenderConfiguration();

	/* Common samplers */
	
	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_LINEAR,      1, &m_linearSamplerState);
	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_ANISOTROPIC, 1, &m_anisotropicSampler);

	RendererConfiguration * r = DX11Module::GetSingleton().GetRendererConfiguration();

	SceneModule * scene  = Game::GetSingleton().GetSceneModule();
	Camera      * camera = scene->GetCamera();

	if (camera)
	{

		/* Camera setup */

		Matrix4 view           = camera->GetViewMatrix();
		Matrix4 projection     = camera->GetProjectionMatrix();
		Matrix4 viewProjection = projection * view;

		MakeCameraBuffer(m_cameraTransformBuffer,
						 view, viewProjection, viewProjection.Inverse(),
						 camera->GetPosition(),
						 camera->GetNearClipDistance(), camera->GetFarClipDistance(), camera->GetFovYRadians(), camera->GetClipAspectRatio());

		m_cameraTransformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, __MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM);
		m_cameraTransformBuffer.Bind(DX11PipelineStage::PIXEL_SHADER,  __MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM);

		auto visibleObjects = scene->GetVisibleObjects(*camera);
		//auto visibleObjects = scene->GetObjectsList();

		DX11RasterizerState backCull({ false, CullMode::BACK });

		auto corners = camera->GetFrustum().GetCorners();

		/* First step (geometry buffer) */

		ID3D11RenderTargetView * gbuffer[] = {
			m_gbuffer0.GetRenderTargetView(),
			m_gbuffer1.GetRenderTargetView()
		};

		m_gbuffer0.ClearRenderTarget(Vector4f(0.f));
		m_gbuffer1.ClearRenderTarget(Vector4f(0.f));

		m_depthBuffer.Clear();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(2, gbuffer, m_depthBuffer.GetDepthStencilView());

		DX11Device::GetSingleton().SetBlending(false);
		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::ON);

		/* Sort the objects to minimize context switching */

		std::vector<GameObject *> visibleObjectsVector(visibleObjects.begin(), visibleObjects.end());

		std::sort(visibleObjectsVector.begin(), visibleObjectsVector.end(),
				  [] (GameObject * a, GameObject * b)
		{
			
			return !a->GetRenderComponent()->GetHeightMap() &&
			       b->GetRenderComponent()->GetHeightMap();

		});

		backCull.Use();

		enum GBufferContextState { MYE_HEIGHT_OFF, MYE_HEIGHT_ON }
		gbufferContextState = MYE_HEIGHT_OFF;

		m_deferredGeometry[0]->Use();

		for (GameObject * object : visibleObjectsVector)
		{

			RenderComponent * rc = object->GetRenderComponent();

			/* Context switch */

			bool sameState, changedState = false;

			do
			{

				switch (gbufferContextState)
				{

				case MYE_HEIGHT_OFF:

					if (rc->GetHeightMap())
					{
						sameState = false;
						gbufferContextState = (GBufferContextState) (gbufferContextState + 1);
						changedState = true;
					}
					else
					{
						sameState = true;
					}

				case MYE_HEIGHT_ON:

					sameState = true;
					break;


				}

			} while (!sameState);

			if (changedState)
			{
				m_deferredGeometry[(int) gbufferContextState]->Use();
			}

			TransformComponent * tc = object->GetTransformComponent();

			GPUBufferPointer gpuBuffer = rc->GetGPUBuffer();

			if (gpuBuffer && gpuBuffer->Load())
			{

				MakeTransformBuffer(m_transformBuffer, tc->GetWorldMatrix() * rc->GetModelMatrix(), view, projection);
				MakeMaterialBuffer(m_materialBuffer, rc->GetMaterial());

				m_transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
				m_materialBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_MATERIAL);

				switch (gbufferContextState)
				{

				case MYE_HEIGHT_ON:

					TexturePointer     heightMap     = rc->GetHeightMap();
					DX11TexturePointer heightMapDX11 = Resource::StaticCast<DX11Texture>(heightMap);;

					heightMapDX11->Load();
					heightMapDX11->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_HEIGHTMAP);

					break;

				}

				DX11VertexBufferPointer vertexBuffer = Resource::StaticCast<DX11VertexBuffer>(gpuBuffer);
				vertexBuffer->Bind();

				DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				DX11Device::GetSingleton().GetImmediateContext()->Draw(vertexBuffer->GetVerticesCount(), 0);

				vertexBuffer->Unbind();

			}

		}

		/* Second step (light buffer) */

		m_lbuffer.ClearRenderTarget(Vector4(.0f));

		ID3D11RenderTargetView * lbuffer = m_lbuffer.GetRenderTargetView();

		m_quadVertexBuffer->Load();

		m_randomCosSin->Load();

		bool lpvEnabled = r->GetLPVEnabled();

		if (lpvEnabled)
		{
			AABB lpvVolume = r->GetLPVAABB();
			m_lpv.Init(camera, lpvVolume.GetMinimum(), lpvVolume.GetMaximum());
			m_rsm.SetVolumeConstraint(lpvVolume);
		}
		else
		{
			m_rsm.SetVolumeConstraint(AABB::FromMinMax(0, 0));
		}

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

		m_gbuffer0.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER0);
		m_gbuffer1.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER1);

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

				m_pssmSliceBuffer.SetSubData(&pssmBufferData[0], 0, m_rsm.GetCSMSplits() * sizeof(detail::PSSMSlice));
				m_pssmSliceBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_PSSMBUFFER);

				//for (int i = 0; i < slices; i++)
				//{
				//	int debugTexSize = 150;
				//	DX11Module::GetSingleton().RenderShaderResource(m_rsm.GetDepthShaderResource(), Vector2i(i * debugTexSize, 0), Vector2i(debugTexSize), i);
				//	//DX11Module::GetSingleton().RenderShaderResource(m_rsm.GetPositionShaderResource(), Vector2i(i * debugTexSize, 0), Vector2i(debugTexSize), i);
				//}
				


			}
			else
			{
				slices = 1;
			}

			/* Render to light buffer */

			m_deferredLights->Use();

			DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

			DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &lbuffer, nullptr);

			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP,     1, &m_shadowMapSamplerState);
			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP, 1, &m_shadowMapSamplerCmpState);
			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_RANDOM,        1, &m_randomSamplerState);			

			m_randomCosSin->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_RANDOM);

			/* Setup constant buffers */

			MakeLightBuffer(m_lightBuffer, light);
			m_lightBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

			DX11Device::GetSingleton().SetBlending(true);

			const Matrix4      & shadowViewProjMatrix = m_rsm.GetLightSpaceTransformMatrix();
			DX11ShaderResource & rsmDepth             = m_rsm.GetDepthShaderResource();

			rsmDepth.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

			m_matrixBuffer.SetData(&shadowViewProjMatrix);
			m_matrixBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM);

			m_configurationBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_RENDERCONFIGURATION);

			/* Render the quad */

			DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_accumulateBlendState, nullptr, 0xFFFFFFFF);

			backCull.Use();

			m_quadVertexBuffer->Bind();

			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DX11Device::GetSingleton().GetImmediateContext()->Draw(m_quadVertexBuffer->GetVerticesCount(), 0);

			m_quadVertexBuffer->Unbind();

			rsmDepth.Unbind();

			m_randomCosSin->Unbind();
			
			if (lpvEnabled)
			{

				/* Inject LPV */

				m_lpv.Inject(m_rsm);

			}
			

		}

		if (lpvEnabled)
		{

			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_LPV, 1, &m_lpvSamplerState);

			m_lpv.Propagate(DX11Module::GetSingleton().GetRendererConfiguration()->GetLPVIterations());

			/* Render LPV */

			/* TODO: SETUP THE SHADERS TO RENDER LPV TO LIGHTBUFFER AND THEIR CONSTANT BUFFERS */

			//m_lbuffer.ClearRenderTarget(Vector4(0));

			m_deferredLightsLPV->Use();

			DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &lbuffer, nullptr);

			m_lpv.BindConfigurationBuffer(DX11PipelineStage::PIXEL_SHADER, 0);

			DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_accumulateBlendState, nullptr, 0xFFFFFFFF);
			DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

			m_lpv.GetLightVolume().Bind(DX11PipelineStage::PIXEL_SHADER,
										__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED,
										__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN,
										__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);

			m_quadVertexBuffer->Bind();

			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DX11Device::GetSingleton().GetImmediateContext()->Draw(m_quadVertexBuffer->GetVerticesCount(), 0);

			m_quadVertexBuffer->Unbind();

			m_gbuffer0.Unbind();
			m_gbuffer1.Unbind();

			m_lpv.GetLightVolume().Unbind();

		}

		m_gbuffer0.Unbind();
		m_gbuffer1.Unbind();

		/* Final pass (shading) */

		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::LOOKUP);

		DX11Device::GetSingleton().GetImmediateContext()->ClearRenderTargetView(target, m_clearColor.Data());
		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &target, m_depthBuffer.GetDepthStencilView());

		m_lbuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_LIGHTBUFFER);

		backCull.Use();

		for (GameObject * object : visibleObjects)
		{

			RenderComponent * rc = object->GetRenderComponent();

			DX11TexturePointer difftex = Resource::StaticCast<DX11Texture>(rc->GetDiffuseTexture());

			if (difftex && difftex->Load())
			{
				m_deferredFinal[1]->Use();
				difftex->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_DIFFUSE);
			}
			else
			{
				m_deferredFinal[0]->Use();
			}

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

		m_lbuffer.Unbind();

		//DX11Module::GetSingleton().RenderShaderResource(m_lbuffer, Vector2i(300, 0), Vector2i(267, 150));

	}

}

bool DX11DeferredLightingRenderer::__CreateConstantBuffers(void)
{

	return m_transformBuffer.Create(sizeof(detail::TransformBuffer)) &&
	       m_lightBuffer.Create(sizeof(detail::LightBuffer)) &&
		   m_cameraTransformBuffer.Create(sizeof(detail::CameraBuffer)) &&
	       m_materialBuffer.Create(sizeof(detail::MaterialBuffer)) &&
	       m_configurationBuffer.Create(sizeof(detail::RenderConfigurationBuffer)) &&
           m_matrixBuffer.Create(sizeof(Matrix4)) &&
		   m_pssmSliceBuffer.Create(sizeof(detail::PSSMSlice) * __MYE_RSM_MAX_CSM_COUNT);

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

	//shadowMapSamplerCmpDesc.Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowMapSamplerCmpDesc.Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	shadowMapSamplerCmpDesc.AddressU       = D3D11_TEXTURE_ADDRESS_MIRROR;
	shadowMapSamplerCmpDesc.AddressV       = D3D11_TEXTURE_ADDRESS_MIRROR;
	shadowMapSamplerCmpDesc.AddressW       = D3D11_TEXTURE_ADDRESS_MIRROR;
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

	lpvSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	//lpvSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	lpvSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
	lpvSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
	lpvSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
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

	D3D11_SAMPLER_DESC linearSamplerDesc;

	linearSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	linearSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	linearSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	linearSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	linearSamplerDesc.MipLODBias     = 0.0f;
	linearSamplerDesc.MaxAnisotropy  = 0;
	linearSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	linearSamplerDesc.BorderColor[0] = 0;
	linearSamplerDesc.BorderColor[1] = 0;
	linearSamplerDesc.BorderColor[2] = 0;
	linearSamplerDesc.BorderColor[3] = 0;
	linearSamplerDesc.MinLOD         = 0;
	linearSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	D3D11_SAMPLER_DESC anisotropicSampler;

	anisotropicSampler.Filter         = D3D11_FILTER_ANISOTROPIC;
	anisotropicSampler.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	anisotropicSampler.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	anisotropicSampler.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	anisotropicSampler.MipLODBias     = 0.0f;
	anisotropicSampler.MaxAnisotropy  = 16;
	anisotropicSampler.ComparisonFunc = D3D11_COMPARISON_LESS;
	anisotropicSampler.BorderColor[0] = 0;
	anisotropicSampler.BorderColor[1] = 0;
	anisotropicSampler.BorderColor[2] = 0;
	anisotropicSampler.BorderColor[3] = 0;
	anisotropicSampler.MinLOD         = 0;
	anisotropicSampler.MaxLOD         = D3D11_FLOAT32_MAX;

	DX11Device::GetSingleton()->CreateSamplerState(&linearSamplerDesc, &m_linearSamplerState);

	return !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&shadowMapSamplerDesc,    &m_shadowMapSamplerState)) &&
	       !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&shadowMapSamplerCmpDesc, &m_shadowMapSamplerCmpState)) &&
		   !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&lpvSamplerDesc,          &m_lpvSamplerState)) &&
		   !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&randomSamplerDesc,       &m_randomSamplerState)) &&
		   !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&linearSamplerDesc,       &m_linearSamplerState)) &&
		   !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&anisotropicSampler,      &m_anisotropicSampler)) &&
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

	//const RenderVariableChange * rEvent = static_cast<const RenderVariableChange *>(event);

	//switch (rEvent->variable)
	//{

	//case RendererVariable::SHADOWMAPRESOLUTION:
	//	m_rsm.SetResolution(rEvent->configuration->GetShadowMapResolution());
	//	break;

	//case RendererVariable::CSMSPLITS:
	//	m_rsm.SetCSMSplits(rEvent->configuration->GetCSMSplits());
	//	break;

	//case RendererVariable::LPVFLUXINJECTIONBIAS:
	//	m_lpv.SetFluxInjectionBias(rEvent->configuration->GetLPVFluxInjectionBias());
	//	break;

	//case RendererVariable::LPVGEOMETRYINJECTIONBIAS:
	//	m_lpv.SetGeometryInjectionBias(rEvent->configuration->GetLPVGeometryInjectionBias());
	//	break;

	//case RendererVariable::RESOLUTION:
	//	
	//	/* Resize buffers */

	//	if (m_initialized)
	//	{

	//		__ResizeBuffers(rEvent->configuration->GetScreenResolution());

	//	}

	//	break;

	//}

	m_renderConfigurationUptodate.store(false);

}

void DX11DeferredLightingRenderer::__UpdateRenderConfiguration(void)
{

	bool bFalse = false;

	if (m_renderConfigurationUptodate.compare_exchange_strong(bFalse, true))
	{

		auto configuration = DX11Module::GetSingleton().GetRendererConfiguration();

		{

			__ResizeBuffers(configuration->GetScreenResolution());

			m_lpv.SetGeometryInjectionBias(configuration->GetLPVGeometryInjectionBias());
			m_lpv.SetFluxInjectionBias(configuration->GetLPVFluxInjectionBias());

			m_rsm.SetResolution(configuration->GetShadowMapResolution());
			m_rsm.SetCSMSplits(configuration->GetCSMSplits());

		}

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