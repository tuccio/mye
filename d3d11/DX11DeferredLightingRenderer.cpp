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

#include <boost/thread.hpp>

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

enum {
	__MYE_SHADER_GEOMETRY_NONE = -1, __MYE_SHADER_GEOMETRY, __MYE_SHADER_GEOMETRY_NHMAP, __MYE_SHADER_GEOMETRY_SPEC, __MYE_SHADER_GEOMETRY_NHMAP_SPEC
};

enum {
	__MYE_SHADER_FINAL_NONE = -1, __MYE_SHADER_FINAL, __MYE_SHADER_FINAL_DIFF, __MYE_SHADER_FINAL_SPEC, __MYE_SHADER_FINAL_DIFF_SPEC
};

#define __MYE_DX11_SSAO_RATIO              .5f
#define __MYE_DX11_PP_RATIO                .5f
#define __MYE_DX11_PP_LUMINANCE_RESOLUTION 1024

DX11DeferredLightingRenderer::DX11DeferredLightingRenderer(void) :
	m_initialized(false),
	m_clearColor(0.f),
	m_vsmEnabled(false),
	m_msaa(MSAA::MSAA_OFF)
{

	Parameters params({ { "renderTarget", "true" } });

	m_gbuffer0.SetParametersList(params);
	m_gbuffer1.SetParametersList(params);
	m_diffuseLight.SetParametersList(params);
	m_specularLight.SetParametersList(params);
	m_deferredOutput.SetParametersList(params);

	m_ppBuffers[0].SetParametersList(params);
	m_ppBuffers[1].SetParametersList(params);

	params.Add("generateMips", "true");

	DX11DepthBufferConfiguration depthBufferConf;

	depthBufferConf.height         = 0;
	depthBufferConf.width          = 0;
	depthBufferConf.shaderResource = true;
	depthBufferConf.arraySize      = 1;
	depthBufferConf.msaa           = MSAA::MSAA_OFF;

	m_depthBuffer = DX11DepthBuffer(depthBufferConf);

}


DX11DeferredLightingRenderer::~DX11DeferredLightingRenderer(void)
{

}

bool DX11DeferredLightingRenderer::Init(void)
{

	auto configuration = static_cast<GraphicsModule *>(Game::GetSingleton().GetGraphicsModule())->GetRendererConfiguration();

	auto resolution = configuration->GetScreenResolution();

	m_rsm.SetResolution(configuration->GetShadowMapResolution());
	m_rsm.SetCSMSplits(configuration->GetCSMSplits());

	if (m_depthBuffer.Create() &&
	    m_rsm.Create() &&
	    m_lpv.Create(configuration->GetLPVResolution(), configuration->GetLPVRSMSamples()) &&
	    m_ssao.Create(resolution.x() * __MYE_DX11_SSAO_RATIO, resolution.y() * __MYE_DX11_SSAO_RATIO) &&
	    __CreateShaders() &&
	    __CreateConstantBuffers() &&
	    __CreateContextStates())
	{

		m_randomCosSin = ResourceTypeManager::GetSingleton().GetResource<DX11Texture>("Texture", "MYE_RANDOM_COS_SIN");

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

		m_diffuseLight.Destroy();
		m_specularLight.Destroy();

		m_ssao.Destroy();
		m_vsm.Destroy();

		m_rsm.Destroy();
		m_lpv.Destroy();

		m_deferredGeometry[0] = nullptr;
		m_deferredGeometry[1] = nullptr;
		m_deferredGeometry[2] = nullptr;
		m_deferredGeometry[3] = nullptr;

		m_deferredLights      = nullptr;
		m_deferredLightsLPV   = nullptr;

		m_deferredFinal[0]    = nullptr;
		m_deferredFinal[1]    = nullptr;
		m_deferredFinal[2]    = nullptr;
		m_deferredFinal[3]    = nullptr;

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

	D3D11_VIEWPORT fullscreenViewport;

	fullscreenViewport.TopLeftX = 0.f;
	fullscreenViewport.TopLeftY = 0.f;
	fullscreenViewport.Width    = m_gbuffer0.GetWidth();
	fullscreenViewport.Height   = m_gbuffer0.GetHeight();
	fullscreenViewport.MinDepth = 0.f;
	fullscreenViewport.MaxDepth = 1.f;

	DX11Device::GetSingleton().SetViewports(&fullscreenViewport, 1);

	/* Common samplers */
	
	DX11Device::GetSingleton().GetImmediateContext()->VSSetSamplers(__MYE_DX11_SAMPLER_SLOT_BILINEAR,    1, &m_bilinearSamplerState);

	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_POINT   ,    1, &m_pointSamplerState);
	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_BILINEAR,    1, &m_bilinearSamplerState);
	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_TRILINEAR,   1, &m_trilinearSamplerState);
	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_ANISOTROPIC, 1, &m_anisotropicSampler);

	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_BLUR,        1, &m_blurSamplerState);

	RendererConfiguration * r = DX11Module::GetSingleton().GetRendererConfiguration();

	SceneModule * scene  = Game::GetSingleton().GetSceneModule();
	Camera      * camera = scene->GetCamera();

	if (camera)
	{

		/* Camera setup */

		Matrix4 view              = camera->GetViewMatrix();
		Matrix4 projection        = camera->GetProjectionMatrix();
		Matrix4 viewProjection    = projection * view;
		Matrix4 invViewProjection = viewProjection.Inverse();

		MakeCameraBuffer(m_cameraTransformBuffer,
		                 view, viewProjection, invViewProjection,
		                 camera->GetPosition(),
		                 camera->GetNearClipDistance(), camera->GetFarClipDistance(), camera->GetFovYRadians(), camera->GetClipAspectRatio());

		m_cameraTransformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, __MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM);
		m_cameraTransformBuffer.Bind(DX11PipelineStage::PIXEL_SHADER,  __MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM);

		auto visibleObjects = scene->GetVisibleObjects(*camera);
		//auto visibleObjects = scene->GetObjectsList();

		DX11RasterizerState backCull({ false, CullMode::BACK });

		auto corners = camera->GetFrustum().GetCorners();

		/* First step (geometry buffer) */

		ID3D11RenderTargetView * gbufferRT[] = {
			m_gbuffer0.GetRenderTargetView(),
			m_gbuffer1.GetRenderTargetView()
		};

		m_gbuffer0.ClearRenderTarget(Vector4f(0.f));
		m_gbuffer1.ClearRenderTarget(Vector4f(0.f));

		m_depthBuffer.Clear();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(2, gbufferRT, m_depthBuffer.GetDepthStencilView());

		DX11Device::GetSingleton().SetBlending(false);
		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::ON);

		/* Sort the objects (just for Z to reduce pixel shader computation, should add material sorting for bandwidth) */

		std::vector<GameObject *> sortedVisibleObjects(visibleObjects.begin(), visibleObjects.end());

		std::unordered_map<GameObject *, int> visibleObjectsSortKeys(sortedVisibleObjects.size());

		std::transform(sortedVisibleObjects.begin(), sortedVisibleObjects.end(),
		               std::inserter(visibleObjectsSortKeys, visibleObjectsSortKeys.begin()),
		[&viewProjection] (GameObject * gameObject) {

			int key;

			Vector3 x = gameObject->GetAABB().GetCenter();

			Vector4 clipPosition = viewProjection * Vector4(x, 1);

			key = (int) (1024.f * clipPosition.z() / clipPosition.w());

			return std::make_pair(gameObject, key);

		});

		std::sort(sortedVisibleObjects.begin(), sortedVisibleObjects.end(),
				  [&visibleObjectsSortKeys] (GameObject * a, GameObject * b)
		{
			return visibleObjectsSortKeys[a] < visibleObjectsSortKeys[b];
		});

		backCull.Use();

		int currentState = __MYE_SHADER_GEOMETRY_NONE;

		for (GameObject * object : sortedVisibleObjects)
		{

			RenderComponent * rc = object->GetRenderComponent();

			/* Context switch */

			TransformComponent * tc = object->GetTransformComponent();

			GPUBufferPointer gpuBuffer = rc->GetGPUBuffer();

			if (gpuBuffer && gpuBuffer->Load())
			{

				MakeTransformBuffer(m_transformBuffer, tc->GetWorldMatrix() * rc->GetModelMatrix(), view, projection);
				MakeMaterialBuffer(m_materialBuffer, rc->GetMaterial());

				m_transformBuffer.Bind(DX11PipelineStage::VERTEX_SHADER, 0);
				m_materialBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_MATERIAL);

				TexturePointer heightMap   = rc->GetNormalHeightMap();
				TexturePointer specularMap = rc->GetSpecularTexture();

				if (heightMap)
				{

					DX11TexturePointer heightMapDX11 = Resource::StaticCast<DX11Texture>(heightMap);

					heightMapDX11->Load();
					heightMapDX11->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_NORMALHEIGHTMAP);

				}

				if (specularMap)
				{

					DX11TexturePointer specularMapDX11 = Resource::StaticCast<DX11Texture>(specularMap);

					specularMapDX11->Load();
					specularMapDX11->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_SPECULAR);

				}

				if (heightMap)
				{

					if (specularMap)
					{

						if (currentState != __MYE_SHADER_GEOMETRY_NHMAP_SPEC)
						{
							m_deferredGeometry[__MYE_SHADER_GEOMETRY_NHMAP_SPEC]->Use();
							currentState = __MYE_SHADER_GEOMETRY_NHMAP_SPEC;
						}

					}
					else if (currentState != __MYE_SHADER_GEOMETRY_NHMAP)
					{
						m_deferredGeometry[__MYE_SHADER_GEOMETRY_NHMAP]->Use();
						currentState = __MYE_SHADER_GEOMETRY_NHMAP;
					}

				}
				else if (specularMap)
				{

					if (currentState != __MYE_SHADER_GEOMETRY_SPEC)
					{
						m_deferredGeometry[__MYE_SHADER_GEOMETRY_SPEC]->Use();
						currentState = __MYE_SHADER_GEOMETRY_SPEC;
					}

				}
				else if (currentState != __MYE_SHADER_GEOMETRY)
				{
					m_deferredGeometry[__MYE_SHADER_GEOMETRY]->Use();
					currentState = __MYE_SHADER_GEOMETRY;
				}
				

				DX11VertexBufferPointer vertexBuffer = Resource::StaticCast<DX11VertexBuffer>(gpuBuffer);
				vertexBuffer->Bind();

				DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				DX11Device::GetSingleton().GetImmediateContext()->Draw(vertexBuffer->GetVerticesCount(), 0);

				vertexBuffer->Unbind();

			}

		}

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

		DX11ShaderResource * gbufferSRV[2];

		if (m_msaa != MSAA::MSAA_OFF)
		{

			m_gbuffer0.Resolve(m_msaaResolveAux[0]);
			m_gbuffer1.Resolve(m_msaaResolveAux[1]);

			gbufferSRV[0] = &m_msaaResolveAux[0];
			gbufferSRV[1] = &m_msaaResolveAux[1];

		}
		else
		{
			gbufferSRV[0] = &m_gbuffer0;
			gbufferSRV[1] = &m_gbuffer1;
		}

		gbufferSRV[0]->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER0);
		gbufferSRV[1]->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER1);

		m_randomCosSin->Load();
		m_randomCosSin->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_RANDOM);

		bool lpvEnabled = r->GetLPVEnabled();


		if (lpvEnabled)
		{

			m_depthBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_GBUFFER2);

			m_ssao.Render();

			m_depthBuffer.Unbind();

			AABB lpvVolume = r->GetLPVAABB();

			m_lpv.Init(camera, lpvVolume.GetMinimum(), lpvVolume.GetMaximum());

			m_rsm.SetAntiFlickeringDelta(m_lpv.GetCellSize());
			m_rsm.SetVolumeConstraint(lpvVolume);

		}
		else
		{
			m_rsm.SetVolumeConstraint(AABB::FromMinMax(0, 0));
			m_rsm.SetAntiFlickeringDelta(0.f);
		}

		

		/* Second step (light buffer) */

		m_diffuseLight.ClearRenderTarget(Vector4(0.f));
		m_specularLight.ClearRenderTarget(Vector4(0.f));

		ID3D11RenderTargetView * lightPassBuffers[2] = { m_diffuseLight.GetRenderTargetView(), m_specularLight.GetRenderTargetView() };

		for (Light * light : scene->GetLightsList())
		{

			/* Generate RSM */

			m_rsm.Render(light);

			int slices;

			std::vector<detail::PSSMSlice> pssmBufferData;

			if (light->GetType() == LightType::DIRECTIONAL)
			{

				slices = m_rsm.GetCSMSplits();

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

			DX11ShaderResource * shadowMap;

			if (m_vsmEnabled)
			{

				auto & depth = m_rsm.GetDepthShaderResource();

				depth.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);
				m_vsm.Render();
				depth.Unbind();

				shadowMap = &m_vsm.GetVSM();

			}
			else
			{

				shadowMap = &m_rsm.GetDepthShaderResource();

			}

			m_deferredLights->Use();

			DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

			DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(2, lightPassBuffers, nullptr);

			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP,     1, &m_shadowMapSamplerState);
			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP, 1, &m_shadowMapSamplerCmpState);
			DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_RANDOM,        1, &m_randomSamplerState);			

			/* Setup constant buffers */

			MakeLightBuffer(m_lightBuffer, light);
			m_lightBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

			DX11Device::GetSingleton().SetBlending(true);

			Matrix4 shadowViewProjMatrix = m_rsm.GetLightSpaceTransformMatrix();

			if (slices == 1)
			{
				shadowViewProjMatrix = m_rsm.GetPSSMCropMatrix(0) * shadowViewProjMatrix;
			}

			shadowMap->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

			m_matrixBuffer.SetData(&shadowViewProjMatrix);
			m_matrixBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM);

			m_configurationBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_RENDERCONFIGURATION);

			/* Render the quad */

			DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_accumulateBlendState, nullptr, 0xFFFFFFFF);

			backCull.Use();

			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

			shadowMap->Unbind();

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

			m_deferredLightsLPV->Use();

			DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, lightPassBuffers, nullptr);

			m_lpv.BindConfigurationBuffer(DX11PipelineStage::PIXEL_SHADER, 0);

			DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_accumulateBlendState, nullptr, 0xFFFFFFFF);
			DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

			m_lpv.GetLightVolume().Bind(DX11PipelineStage::PIXEL_SHADER,
										__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED,
										__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN,
										__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);

			DX11ShaderResource & occlusion = m_ssao.GetSSAOBuffer();

			occlusion.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_OCCLUSION);

			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

			occlusion.Unbind();

			m_lpv.GetLightVolume().Unbind();

		}

		gbufferSRV[0]->Unbind();
		gbufferSRV[1]->Unbind();

		/* Final pass (shading) */

		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::LOOKUP);
		DX11Device::GetSingleton().SetBlending(false);

		m_deferredOutput.ClearRenderTarget(0);

		ID3D11RenderTargetView * deferredOutput = m_deferredOutput.GetRenderTargetView();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &deferredOutput, m_depthBuffer.GetDepthStencilView());

		m_diffuseLight.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_LIGHTDIFFUSE);
		m_specularLight.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_LIGHTSPECULAR);

		backCull.Use();

		DX11Device::GetSingleton().SetBlending(false);

		currentState = __MYE_SHADER_FINAL_NONE;

		for (GameObject * object : sortedVisibleObjects)
		{

			RenderComponent * rc = object->GetRenderComponent();

			DX11TexturePointer difftex = Resource::StaticCast<DX11Texture>(rc->GetDiffuseTexture());
			DX11TexturePointer spectex = Resource::StaticCast<DX11Texture>(rc->GetSpecularTexture());

			if (difftex && difftex->Load())
			{
				difftex->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_DIFFUSE);
			}

			if (spectex && spectex->Load())
			{
				difftex->Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_SPECULAR);
			}

			if (difftex)
			{

				if (spectex)
				{
					if (currentState != __MYE_SHADER_FINAL_DIFF_SPEC)
					{
						m_deferredFinal[__MYE_SHADER_FINAL_DIFF_SPEC]->Use();
						currentState = __MYE_SHADER_FINAL_DIFF_SPEC;
					}
				}
				else if (currentState != __MYE_SHADER_FINAL_DIFF)
				{
					m_deferredFinal[__MYE_SHADER_FINAL_DIFF]->Use();
					currentState = __MYE_SHADER_FINAL_DIFF;
				}
				
			}
			else if (spectex)
			{
				if (currentState != __MYE_SHADER_FINAL_SPEC)
				{
					m_deferredFinal[__MYE_SHADER_FINAL_SPEC]->Use();
					currentState = __MYE_SHADER_FINAL_SPEC;
				}
			}
			else if (currentState != __MYE_SHADER_FINAL)
			{
				m_deferredFinal[__MYE_SHADER_FINAL]->Use();
				currentState = __MYE_SHADER_FINAL;
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

		m_diffuseLight.Unbind();
		m_specularLight.Unbind();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);
		DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

		DX11Texture * frame;

		if (m_msaa == MSAA::MSAA_OFF)
		{
			frame = &m_deferredOutput;
		}
		else
		{
			m_deferredOutput.Resolve(m_msaaResolveAux[0]);
			frame = &m_msaaResolveAux[0];
		}

		frame->Bind(DX11PipelineStage::PIXEL_SHADER, 0);

		DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		/* Bloom */

		ID3D11RenderTargetView * ppBuffersRTV[] = { m_ppBuffers[0].GetRenderTargetView(), m_ppBuffers[1].GetRenderTargetView() };

		D3D11_VIEWPORT ppViewport;

		ppViewport.TopLeftX = 0.f;
		ppViewport.TopLeftY = 0.f;
		ppViewport.Width    = m_ppBuffers[0].GetWidth();
		ppViewport.Height   = m_ppBuffers[0].GetHeight();
		ppViewport.MinDepth = 0.f;
		ppViewport.MaxDepth = 1.f;

		DX11Device::GetSingleton().SetViewports(&ppViewport, 1);

		m_ppBloomThreshold->Use();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &ppBuffersRTV[0], nullptr);

		DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

		m_ppBloomBlur[0]->Use();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &ppBuffersRTV[1], nullptr);
		m_ppBuffers[0].Bind(DX11PipelineStage::PIXEL_SHADER, 1);

		DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

		m_ppBuffers[0].Unbind();

		m_ppBloomBlur[1]->Use();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &ppBuffersRTV[0], nullptr);
		m_ppBuffers[1].Bind(DX11PipelineStage::PIXEL_SHADER, 1);

		DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

		m_ppBuffers[1].Unbind();

		DX11Texture * bloomedFrame = &m_specularLight;
		ID3D11RenderTargetView * bloomedFrameRTV = bloomedFrame->GetRenderTargetView();

		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &bloomedFrameRTV, nullptr);

		m_ppBuffers[0].Bind(DX11PipelineStage::PIXEL_SHADER, 1);

		/* Render bloom on full screen */

		DX11Device::GetSingleton().SetViewports(&fullscreenViewport, 1);

		m_ppBloomCombine->Use();
		
		DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

		m_ppBuffers[0].Unbind();

		frame->Unbind();

		/* Tonemap */


		m_tonemapping->Use();
		
		DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &target, nullptr);

		bloomedFrame->Bind(DX11PipelineStage::PIXEL_SHADER, 0);

		DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

		bloomedFrame->Unbind();

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

	m_pointSamplerState        = CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);
	m_bilinearSamplerState     = CreateSamplerState(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);
	m_trilinearSamplerState    = CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	m_anisotropicSampler       = CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 8);

	m_lpvSamplerState          = CreateSamplerState(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);
	m_blurSamplerState         = CreateSamplerState(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);
	m_randomSamplerState       = CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);

	m_shadowMapSamplerState    = CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_BORDER);
	m_shadowMapSamplerCmpState = CreateSamplerState(D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_BORDER, 0, D3D11_COMPARISON_LESS);

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

	accumulateBlendStateDesc.IndependentBlendEnable = FALSE;

	return m_pointSamplerState && m_bilinearSamplerState && m_trilinearSamplerState &&
	       m_lpvSamplerState && m_shadowMapSamplerState && m_shadowMapSamplerCmpState &&
	       m_blurSamplerState && 
	       !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateBlendState(&accumulateBlendStateDesc, &m_accumulateBlendState));

}

void DX11DeferredLightingRenderer::__DestroyContextStates(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_pointSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_bilinearSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_trilinearSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_anisotropicSampler);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shadowMapSamplerCmpState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shadowMapSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_lpvSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_blurSamplerState);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_randomSamplerState);
}

void DX11DeferredLightingRenderer::OnEvent(const IEvent * event)
{

	boost::lock_guard<boost::mutex> lock(m_renderVariablesChangedMutex);

	const RenderVariableChange * rEvent = static_cast<const RenderVariableChange *>(event);

	m_renderVariablesChanged.push_back(rEvent->variable);

}

void DX11DeferredLightingRenderer::__UpdateRenderConfiguration(void)
{

	boost::lock_guard<boost::mutex> lock(m_renderVariablesChangedMutex);

	if (!m_renderVariablesChanged.empty())
	{

		auto configuration = DX11Module::GetSingleton().GetRendererConfiguration();

		for (auto variable : m_renderVariablesChanged)
		{

			switch (variable)
			{

			case RendererVariable::SHADOWMAPRESOLUTION:
			{

				int    shadowMapResolution = configuration->GetShadowMapResolution();
				String smResStr            = ToString(shadowMapResolution);

				m_rsm.SetResolution(shadowMapResolution);
				m_vsm.SetResolution(shadowMapResolution);

			}
				
				break;

			case RendererVariable::CSMSPLITS:
			{
				
				int splits = configuration->GetCSMSplits();
				m_rsm.SetCSMSplits(splits);

				if (splits == 1)
				{
					m_deferredLights->RemoveMacroDefinition("MYE_SHADOW_MAP_CSM");
				}
				else
				{
					m_deferredLights->AddMacroDefinition("MYE_SHADOW_MAP_CSM");
				}

				m_deferredLights->Load();

				m_vsm.SetSplits(splits);
				
			}
				break;

			case RendererVariable::LPVFLUXINJECTIONBIAS:
				m_lpv.SetFluxInjectionBias(configuration->GetLPVFluxInjectionBias());
				break;

			case RendererVariable::LPVGEOMETRYINJECTIONBIAS:
				m_lpv.SetGeometryInjectionBias(configuration->GetLPVGeometryInjectionBias());
				break;

			case RendererVariable::LPVRSMSAMPLES:
				m_lpv.SetRSMSamples(configuration->GetLPVRSMSamples());;
				break;

			case RendererVariable::LPVRESOLUTION:
				m_lpv.Destroy();
				m_lpv.Create(configuration->GetLPVResolution());
				break;

			case RendererVariable::RESOLUTION:

				__ResizeBuffers(configuration->GetScreenResolution());

				break;

			case RendererVariable::PCFENABLED:

				if (configuration->GetPCFEnabled())
				{
					m_deferredLights->AddMacroDefinition("MYE_SHADOW_MAP_PCF");
				}
				else
				{
					m_deferredLights->RemoveMacroDefinition("MYE_SHADOW_MAP_PCF");
				}

				m_deferredLights->Load();

				break;

			case RendererVariable::VSMENABLED:

				if (configuration->GetVSMEnabled())
				{

					if (m_vsm.Create())
					{
						m_deferredLights->AddMacroDefinition("MYE_SHADOW_MAP_VSM");
						m_vsmEnabled = true;
					}

				}
				else
				{
					m_deferredLights->RemoveMacroDefinition("MYE_SHADOW_MAP_VSM");
					m_vsm.Destroy();
					m_vsmEnabled = false;
				}

				m_deferredLights->Load();

				break;

			case RendererVariable::VSMEXPONENTIAL:

				if (configuration->GetVSMExponential())
				{

					

					m_deferredLights->AddMacroDefinition("MYE_SHADOW_MAP_EVSM");

					m_vsm.SetExponentialTest(true);

				}
				else
				{
					m_deferredLights->RemoveMacroDefinition("MYE_SHADOW_MAP_EVSM");
					m_vsm.SetExponentialTest(false);
				}

				m_deferredLights->Load();

				break;

			case RendererVariable::MSAA:

			{

				Parameters gbParams = m_gbuffer0.GetParametersList();
				Parameters doParams = m_deferredOutput.GetParametersList();

				int    msaaSamples = configuration->GetMSAA();
				String msaa        = ToString(msaaSamples);

				gbParams.Add("msaa", msaa);
				doParams.Add("msaa", msaa);

				m_gbuffer0.SetParametersList(gbParams);
				m_gbuffer1.SetParametersList(gbParams);

				m_deferredOutput.SetParametersList(doParams);

				m_gbuffer0.Destroy();
				m_gbuffer1.Destroy();
				m_deferredOutput.Destroy();

				auto size = configuration->GetScreenResolution();

				m_gbuffer0.Create(size.x(), size.y(), DataFormat::HALF4);
				m_gbuffer1.Create(size.x(), size.y(), DataFormat::HALF4);
				m_deferredOutput.Create(size.x(), size.y(), DataFormat::HALF4);

				m_depthBuffer.SetMSAA(static_cast<MSAA>(msaaSamples));

				if (msaaSamples == 0)
				{
					m_msaaResolveAux[0].Destroy();
					m_msaaResolveAux[1].Destroy();
				}

				m_msaa = static_cast<MSAA>(msaaSamples);

				break;

			}

			}

			

		}

		m_renderVariablesChanged.clear();

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

		r.esmPositiveExponent       = configuration->GetESMPositiveExponent();
		r.esmNegativeExponent       = configuration->GetESMNegativeExponent();

		r.csmSplits                 = configuration->GetCSMSplits();
		r.csmDebug                  = configuration->GetCSMDebug();

		r.pcfEnabled                = configuration->GetPCFEnabled();
		r.pcfKernel                 = configuration->GetPCFKernel();
		r.pcfKernelInvSquare        = 1.f / (r.pcfKernel * r.pcfKernel);

		r.lpvAttenuation            = configuration->GetLPVAttenuation();

		r.ppBloomThreshold          = configuration->GetPPBloomThreshold();
		r.ppBloomPower              = configuration->GetPPBloomPower();

		m_configurationBuffer.SetData(&r);

	}

}

void DX11DeferredLightingRenderer::__ResizeBuffers(const Vector2i & size)
{

	m_gbuffer0.Destroy();
	m_gbuffer1.Destroy();

	m_diffuseLight.Destroy();
	m_specularLight.Destroy();
	
	m_deferredOutput.Destroy();

	m_ssao.Destroy();

	m_gbuffer0.Create(size.x(), size.y(), DataFormat::HALF4);
	m_gbuffer1.Create(size.x(), size.y(), DataFormat::HALF4);

	m_diffuseLight.Create(size.x(), size.y(), DataFormat::HALF4);
	m_specularLight.Create(size.x(), size.y(), DataFormat::HALF4);

	m_deferredOutput.Create(size.x(), size.y(), DataFormat::HALF4);

	m_ppBuffers[0].Create(size.x() * __MYE_DX11_PP_RATIO, size.y() * __MYE_DX11_PP_RATIO, DataFormat::HALF4);
	m_ppBuffers[1].Create(size.x() * __MYE_DX11_PP_RATIO, size.y() * __MYE_DX11_PP_RATIO, DataFormat::HALF4);

	m_ssao.Create(size.x() * __MYE_DX11_SSAO_RATIO, size.y() * __MYE_DX11_SSAO_RATIO);

	m_depthBuffer.Resize(size.x(), size.y());

}

bool DX11DeferredLightingRenderer::__CreateShaders(void)
{

	m_deferredGeometry[__MYE_SHADER_GEOMETRY] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                                                  "./shaders/deferred_lighting_geometry.msh",
	                                                                                                                  nullptr,
	                                                                                                                  { { "type", "program" } });

	m_deferredGeometry[__MYE_SHADER_GEOMETRY_NHMAP] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                                                        "./shaders/deferred_lighting_geometry_nhmap.msh",
	                                                                                                                        nullptr,
	                                                                                                                        { { "type", "program" } });

	m_deferredGeometry[__MYE_SHADER_GEOMETRY_SPEC] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                                                       "./shaders/deferred_lighting_geometry_spec.msh",
	                                                                                                                       nullptr,
	                                                                                                                       { { "type", "program" } });

	m_deferredGeometry[__MYE_SHADER_GEOMETRY_NHMAP_SPEC] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                                                             "./shaders/deferred_lighting_geometry_nhmap_spec.msh",
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

	m_deferredFinal[__MYE_SHADER_FINAL] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                                            "./shaders/deferred_lighting_final.msh",
	                                                                                                            nullptr,
	                                                                                                            { { "type", "program" } });

	m_deferredFinal[__MYE_SHADER_FINAL_DIFF] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                                                 "./shaders/deferred_lighting_final_diff.msh",
	                                                                                                                 nullptr,
	                                                                                                                 { { "type", "program" } });

	m_deferredFinal[__MYE_SHADER_FINAL_SPEC] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                                                 "./shaders/deferred_lighting_final_spec.msh",
	                                                                                                                 nullptr,
	                                                                                                                 { { "type", "program" } });

	m_deferredFinal[__MYE_SHADER_FINAL_DIFF_SPEC] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                                                      "./shaders/deferred_lighting_final_diff_spec.msh",
	                                                                                                                      nullptr,
	                                                                                                                      { { "type", "program" } });

	m_ppBloomThreshold = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                           "./shaders/pp_bloom_threshold.msh",
	                                                                                           nullptr,
	                                                                                           { { "type", "program" } });

	m_ppBloomBlur[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                         "./shaders/pp_bloom_blur_h.msh",
	                                                                                         nullptr,
	                                                                                         { { "type", "program" } });

	m_ppBloomBlur[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                         "./shaders/pp_bloom_blur_v.msh",
	                                                                                         nullptr,
	                                                                                         { { "type", "program" } });

	m_ppBloomCombine = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                         "./shaders/pp_bloom_combine.msh",
	                                                                                         nullptr,
	                                                                                         { { "type", "program" } });
	                                                                                      
	m_tonemapping = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>("DX11Shader",
	                                                                                      "./shaders/tonemapping.msh",
	                                                                                      nullptr,
	                                                                                      { { "type", "program" } });

	return m_deferredGeometry[0]->Load() &&
	       m_deferredGeometry[1]->Load() &&
	       m_deferredGeometry[2]->Load() &&
	       m_deferredGeometry[3]->Load() &&
	       m_deferredLights->Load() &&
	       m_deferredLightsLPV->Load() &&
	       m_deferredFinal[0]->Load() &&
	       m_deferredFinal[1]->Load() &&
	       m_deferredFinal[2]->Load() &&
	       m_deferredFinal[3]->Load() &&
	       m_ppBloomThreshold->Load() &&
	       m_ppBloomBlur[0]->Load() &&
	       m_ppBloomBlur[1]->Load() &&
	       m_ppBloomCombine->Load() &&
	       m_tonemapping->Load();

}