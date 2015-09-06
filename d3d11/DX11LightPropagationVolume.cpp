#include "DX11LightPropagationVolume.h"
#include "DX11VertexBuffer.h"
#include "DX11Defines.h"

#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include "DX11Module.h"

#include "detail/ShadersBuffers.h"

using namespace mye::core;
using namespace mye::dx11;
using namespace mye::math;

DX11LightPropagationVolume::DX11LightPropagationVolume(void) :
	m_currentVolume(0),
	m_geometryInjectionBias(.0f),
	m_fluxInjectionBias(.5f) { }

bool DX11LightPropagationVolume::Create(size_t dimensions, size_t resolution)
{

	m_volumeResolution = dimensions;
	m_rsmSamples       = resolution;

	Parameters textureParams = {
		{ "renderTarget", "true" }
	};

	Parameters volumeParams = {
		{ "type", "3d" },
		{ "depth", ToString(dimensions) },
		{ "renderTarget", "true" }
	};

	/*m_position.SetParametersList(textureParams);
	m_normal.SetParametersList(textureParams);
	m_flux.SetParametersList(textureParams);
	m_depth.SetParametersList(textureParams);*/

	m_quadVertexBuffer = ResourceTypeManager::GetSingleton().GetResource<DX11VertexBuffer>("GPUBuffer", "MYE_QUAD");

	return	/*m_position.Create(resolution, resolution, DataFormat::HALF4) &&
	        m_normal.Create(resolution, resolution,   DataFormat::HALF4) &&
	        m_flux.Create(resolution, resolution,     DataFormat::HALF4) &&
	        m_depth.Create(resolution, resolution,    DataFormat::FLOAT) &&*/
			m_lightVolume[0].Create(dimensions) &&
			m_lightVolume[1].Create(dimensions) &&
			m_geometryVolume.Create(dimensions) &&
			__CreateBuffers() &&
			__CreateShaders() &&
	        __CreateContextStates();

}

void DX11LightPropagationVolume::Destroy(void)
{

	/*m_position.Destroy();
	m_normal.Destroy();
	m_flux.Destroy();*/

	m_lightVolume[0].Destroy();
	m_lightVolume[1].Destroy();

	m_geometryVolume.Destroy();

	__DestroyShaders();
	__DestroyBuffers();
	__DestroyContextStates();

}

void DX11LightPropagationVolume::Init(const Camera * camera, const Vector3 & volumeMinCorner, const Vector3 & volumeMaxCorner)
{

	m_lightVolume[0].Clear();
	m_lightVolume[1].Clear();

	m_geometryVolume.Clear();

	Vector3 diffCorner  = volumeMaxCorner - volumeMinCorner;
	Vector3 center      = .5f * diffCorner + volumeMinCorner;

	Real    extents     = Max(diffCorner.x(), diffCorner.y(), diffCorner.z());
	AABB    volumeAABB  = AABB::FromCenterHalfExtents(center, .5f * extents);

	m_volumeCellSize  = extents / m_volumeResolution;
	m_volumeMinCorner = volumeAABB.GetMinimum();
	m_volumeMaxCorner = volumeAABB.GetMaximum();

	MakeLPVConfigurationBuffer(m_lpvConfig, m_volumeMinCorner, m_volumeMaxCorner, m_volumeCellSize, m_rsmSamples, m_volumeResolution, m_geometryInjectionBias, m_fluxInjectionBias);

}

void DX11LightPropagationVolume::Inject(DX11ReflectiveShadowMap & rsm)
{

	/* Setup the viewport for the RSM downsample */

	auto oldViewports = DX11Device::GetSingleton().GetViewports();

	D3D11_VIEWPORT viewport;

	viewport.Width    = m_rsmSamples;
	viewport.Height   = m_rsmSamples;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	//DX11Device::GetSingleton().SetViewports(&viewport, 1);

	///* RSM Downsampling */

	//DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);
	//DX11Device::GetSingleton().SetBlending(false);

	//ID3D11RenderTargetView * rsmRenderTargets[] = {
	//	m_position.GetRenderTargetView(),
	//	m_normal.GetRenderTargetView(),
	//	m_flux.GetRenderTargetView(),
	//	m_depth.GetRenderTargetView()
	//};

	//DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(4, rsmRenderTargets, nullptr);

	//m_quadVertexBuffer->Load();

	//m_lpvRSMSampling->Use();

	//Light * light = rsm.GetLight();

	//MakeLightBuffer(m_lightBuffer, light);

	//m_lightBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

	//rsm.GetPositionShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
	//rsm.GetNormalShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER,   __MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
	//rsm.GetFluxShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER,     __MYE_DX11_TEXTURE_SLOT_RSMFLUX);
	//rsm.GetDepthShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER,    __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

	/*m_quadVertexBuffer->Bind();

	DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11Device::GetSingleton().GetImmediateContext()->Draw(6, 0);

	m_quadVertexBuffer->Unbind();

	rsm.GetPositionShaderResource().Unbind();
	rsm.GetNormalShaderResource().Unbind();
	rsm.GetFluxShaderResource().Unbind();
	rsm.GetDepthShaderResource().Unbind();

	DX11Module::GetSingleton().RenderShaderResource(m_flux, Vector2i(150, 0), Vector2i(150));*/

	/* Injection */

	m_lightVolume[0].SetRenderTarget();

	m_lpvInjectFlux->Use();

	/*m_position.Bind(DX11PipelineStage::VERTEX_SHADER, __MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
	m_normal.Bind(DX11PipelineStage::VERTEX_SHADER,   __MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
	m_flux.Bind(DX11PipelineStage::VERTEX_SHADER,     __MYE_DX11_TEXTURE_SLOT_RSMFLUX);*/

	viewport.Height   = m_volumeResolution;
	viewport.Width    = m_volumeResolution;

	rsm.GetPositionShaderResource().Bind(DX11PipelineStage::VERTEX_SHADER, __MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
	rsm.GetNormalShaderResource().Bind(DX11PipelineStage::VERTEX_SHADER,   __MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
	rsm.GetFluxShaderResource().Bind(DX11PipelineStage::VERTEX_SHADER,     __MYE_DX11_TEXTURE_SLOT_RSMFLUX);

	DX11Device::GetSingleton().SetViewports(&viewport, 1);

	BindConfigurationBuffer(DX11PipelineStage::VERTEX_SHADER, 0);

	DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_additiveBlend, nullptr, 0xFFFFFFFF);

	unsigned int numSamples = m_rsmSamples * m_rsmSamples;

	// No buffers bound, vertices are generated through SV_VertexID

	DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	DX11Device::GetSingleton().GetImmediateContext()->Draw(numSamples, 0);

	m_geometryVolume.SetRenderTarget();

	BindConfigurationBuffer(DX11PipelineStage::PIXEL_SHADER, 0);

	m_lpvInjectGeometry->Use();

	DX11Device::GetSingleton().GetImmediateContext()->Draw(numSamples, 0);

	rsm.GetPositionShaderResource().Unbind();
	rsm.GetNormalShaderResource().Unbind();
	rsm.GetFluxShaderResource().Unbind();

	/*m_position.Unbind();
	m_normal.Unbind();
	m_flux.Unbind();*/

	m_lpvInjectGeometry->Dispose();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);
	DX11Device::GetSingleton().SetViewports(&oldViewports[0], oldViewports.size());

}

void DX11LightPropagationVolume::BindConfigurationBuffer(DX11PipelineStage stage, unsigned int slot)
{
	m_lpvConfig.Bind(stage, slot);
}

void DX11LightPropagationVolume::Propagate(unsigned int iterations)
{

	DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_additiveBlend, nullptr, 0xFFFFFFFF);

	m_currentVolume = 0;

	DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);
	DX11Device::GetSingleton().SetBlending(false);

	auto oldViewports = DX11Device::GetSingleton().GetViewports();

	D3D11_VIEWPORT viewport;

	viewport.Width    = m_volumeResolution;
	viewport.Height   = m_volumeResolution;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	DX11Device::GetSingleton().SetViewports(&viewport, 1);

	m_lpvPropagateFirst->Use();

	unsigned int verticesCount = m_volumeResolution * m_volumeResolution * m_volumeResolution;

	m_geometryVolume.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_LPVGEOMETRY);
	BindConfigurationBuffer(DX11PipelineStage::VERTEX_SHADER, 0);

	/* First propagation pass with no occlusion */

	//m_lightVolume[1].Clear();
	m_lightVolume[1].SetRenderTarget();

	m_lightVolume[0].Bind(DX11PipelineStage::PIXEL_SHADER,
	                      __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED,
	                      __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN,
	                      __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);

	DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	DX11Device::GetSingleton().GetImmediateContext()->Draw(verticesCount, 0);

	m_lightVolume[0].Unbind();

	m_currentVolume = 1;

	/* Then propagate using the occlusion shader */

	m_lpvPropagate->Use();

	for (unsigned int i = 1; i < iterations; i++)
	{

		uint8_t nextVolume = (i + 1) % 2;

		//m_lightVolume[nextVolume].Clear();
		m_lightVolume[nextVolume].SetRenderTarget();

		m_lightVolume[m_currentVolume].Bind(DX11PipelineStage::PIXEL_SHADER,
		                                    __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED,
		                                    __MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN,
											__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);		

		DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		DX11Device::GetSingleton().GetImmediateContext()->Draw(verticesCount, 0);

		m_lightVolume[m_currentVolume].Unbind();

		m_currentVolume = nextVolume;

	}

	m_geometryVolume.Unbind();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

	m_lpvPropagate->Dispose();

	DX11Device::GetSingleton().SetViewports(&oldViewports[0], oldViewports.size());

}

Real DX11LightPropagationVolume::GetGeometryInjectionBias(void) const
{
	return m_geometryInjectionBias;
}

void DX11LightPropagationVolume::SetGeometryInjectionBias(Real bias)
{
	m_geometryInjectionBias = bias;
}

Real DX11LightPropagationVolume::GetFluxInjectionBias(void) const
{
	return m_fluxInjectionBias;
}

void DX11LightPropagationVolume::SetFluxInjectionBias(Real bias)
{
	m_fluxInjectionBias = bias;
}

size_t DX11LightPropagationVolume::GetRSMSamples(void) const
{
	return m_rsmSamples;
}

void DX11LightPropagationVolume::SetRSMSamples(size_t samples)
{
	m_rsmSamples = samples;
}

bool DX11LightPropagationVolume::__CreateShaders(void)
{


	m_lpvRSMSampling = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/lpv_rsmsampler.msh",
		nullptr,
		{ { "type", "program" } }
	);

	m_lpvInjectFlux = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/lpv_injectflux.msh",
		nullptr,
		{ { "type", "program" } }
	);

	m_lpvInjectGeometry = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/lpv_injectgeometry.msh",
		nullptr,
		{ { "type", "program" } }
	);

	m_lpvPropagate = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/lpv_propagate.msh",
		nullptr,
		{ { "type", "program" } }
	);

	m_lpvPropagateFirst = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/lpv_propagate_first.msh",
		nullptr,
		{ { "type", "program" } }
	);
	
	return m_lpvRSMSampling->Load() &&
	       m_lpvInjectFlux->Load() &&
	       m_lpvInjectGeometry->Load() &&
	       m_lpvPropagateFirst->Load() &&
	       m_lpvPropagate->Load();

}

void DX11LightPropagationVolume::__DestroyShaders(void)
{
	m_lpvRSMSampling    = nullptr;
	m_lpvInjectFlux     = nullptr;
	m_lpvInjectGeometry = nullptr;
	m_lpvPropagate      = nullptr;
	m_lpvPropagateFirst = nullptr;
}

bool DX11LightPropagationVolume::__CreateBuffers(void)
{
	return m_lightBuffer.Create(sizeof(detail::LightBuffer)) &&
	       m_lpvConfig.Create(sizeof(detail::LPVConfiguration));
}

void DX11LightPropagationVolume::__DestroyBuffers(void)
{
	m_lightBuffer.Destroy();
	m_lpvConfig.Destroy();
}

bool DX11LightPropagationVolume::__CreateContextStates(void)
{

	D3D11_BLEND_DESC additiveBlendDesc;

	additiveBlendDesc.AlphaToCoverageEnable                 = FALSE;
	additiveBlendDesc.IndependentBlendEnable                = FALSE;
	additiveBlendDesc.RenderTarget[0].BlendEnable           = TRUE;
	additiveBlendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	additiveBlendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	additiveBlendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateBlendState(&additiveBlendDesc, &m_additiveBlend));

}

void DX11LightPropagationVolume::__DestroyContextStates(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_additiveBlend);
}

/* Light volume */

DX11LightVolume::DX11LightVolume(void) { }

bool DX11LightVolume::Create(size_t resolution)
{

	Parameters volumeParams = {
		{ "type", "3d" },
		{ "depth", ToString(resolution) },
		{ "renderTarget", "true" }
	};

	m_shR.SetParametersList(volumeParams);
	m_shG.SetParametersList(volumeParams);
	m_shB.SetParametersList(volumeParams);

	return m_shR.Create(resolution, resolution, DataFormat::HALF4) &&
	       m_shG.Create(resolution, resolution, DataFormat::HALF4) &&
	       m_shB.Create(resolution, resolution, DataFormat::HALF4);

}

void DX11LightVolume::Destroy(void)
{
	m_shR.Destroy();
	m_shG.Destroy();
	m_shB.Destroy();
}

std::array<ID3D11RenderTargetView *, 3> DX11LightVolume::GetRenderTargetViews(void)
{
	return {
		m_shR.GetRenderTargetView(),
		m_shG.GetRenderTargetView(),
		m_shB.GetRenderTargetView()
	};
}

void DX11LightVolume::SetRenderTarget(void)
{

	ID3D11RenderTargetView * rt[] = {
		m_shR.GetRenderTargetView(),
		m_shG.GetRenderTargetView(),
		m_shB.GetRenderTargetView()
	};

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(3, rt, nullptr);

}

void DX11LightVolume::Bind(DX11PipelineStage stage, int redSlot, int greenSlot, int blueSlot)
{
	m_shR.Bind(stage, redSlot);
	m_shG.Bind(stage, greenSlot);
	m_shB.Bind(stage, blueSlot);
}

void DX11LightVolume::Unbind(void)
{
	m_shR.Unbind();
	m_shG.Unbind();
	m_shB.Unbind();
}

void DX11LightVolume::Clear(const Vector4f & color)
{
	m_shR.ClearRenderTarget(color);
	m_shG.ClearRenderTarget(color);
	m_shB.ClearRenderTarget(color);
}

/* Geometry volume */

DX11GeometryVolume::DX11GeometryVolume(void) { }

bool DX11GeometryVolume::Create(size_t resolution)
{

	Parameters volumeParams = {
			{ "type", "3d" },
			{ "depth", ToString(resolution) },
			{ "renderTarget", "true" }
	};

	m_geometryVolume.SetParametersList(volumeParams);

	return m_geometryVolume.Create(resolution, resolution, DataFormat::HALF4);

}

void DX11GeometryVolume::Destroy(void)
{
	m_geometryVolume.Destroy();
}

ID3D11RenderTargetView * DX11GeometryVolume::GetRenderTargetView(void)
{
	return m_geometryVolume.GetRenderTargetView();
}

void DX11GeometryVolume::SetRenderTarget(void)
{

	ID3D11RenderTargetView * rt = m_geometryVolume.GetRenderTargetView();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &rt, nullptr);

}

void DX11GeometryVolume::Bind(DX11PipelineStage stage, int slot)
{
	m_geometryVolume.Bind(stage, slot);
}

void DX11GeometryVolume::Unbind(void)
{
	m_geometryVolume.Unbind();
}

void DX11GeometryVolume::Clear(const Vector4f & color)
{
	m_geometryVolume.ClearRenderTarget(color);
}