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
	m_currentVolume(0) { }

bool DX11LightPropagationVolume::Create(size_t dimensions, size_t resolution)
{

	m_volumeResolution = dimensions;
	m_sampleResolution = resolution;

	Parameters textureParams = {
		{ "renderTarget", "true" }
	};

	Parameters volumeParams = {
		{ "type", "3d" },
		{ "depth", ToString(dimensions) },
		{ "renderTarget", "true" }
	};

	m_position.SetParametersList(textureParams);
	m_normal.SetParametersList(textureParams);
	m_flux.SetParametersList(textureParams);
	m_depth.SetParametersList(textureParams);

	return	m_position.Create(resolution, resolution, DataFormat::FLOAT4) &&
	        m_normal.Create(resolution, resolution, DataFormat::FLOAT4) &&
	        m_flux.Create(resolution, resolution, DataFormat::FLOAT4) &&
	        m_depth.Create(resolution, resolution, DataFormat::FLOAT) &&
			m_lightVolume[0].Create(dimensions) &&
			m_lightVolume[1].Create(dimensions) &&
			m_geometryVolume.Create(dimensions) &&
			__CreateBuffers() &&
			__CreateShaders() &&
	        __CreateContextStates();

}

void DX11LightPropagationVolume::Destroy(void)
{

	m_position.Destroy();
	m_normal.Destroy();
	m_flux.Destroy();

	m_lightVolume[0].Destroy();
	m_lightVolume[1].Destroy();

	m_geometryVolume.Destroy();

	__DestroyShaders();
	__DestroyBuffers();
	__DestroyContextStates();

}

void DX11LightPropagationVolume::Init(const Camera * camera, const Vector3 & volumeMinCorner, const Vector3 & volumeMaxCorner)
{
	
	Matrix4 viewProjection = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	m_cameraTransformBuffer.SetData(viewProjection.Data());

	m_lightVolume[0].Clear();
	m_lightVolume[1].Clear();

	Vector3 diffCorner  = volumeMaxCorner - volumeMinCorner;
	Vector3 center      = .5f * diffCorner + volumeMinCorner;

	Real    extents     = Max(diffCorner.x(), diffCorner.y(), diffCorner.z());
	AABB    volumeAABB  = AABB::FromCenterHalfExtents(center, .5f * extents);

	m_volumeCellSize  = extents / m_volumeResolution;
	m_volumeMinCorner = volumeAABB.GetMinimum();
	m_volumeMaxCorner = volumeAABB.GetMaximum();

	MakeLPVConfigurationBuffer(m_lpvConfig, m_volumeMinCorner, m_volumeMaxCorner, m_volumeCellSize, m_sampleResolution, m_volumeResolution);

}

void DX11LightPropagationVolume::Inject(DX11ReflectiveShadowMap & rsm)
{

	/* Setup the viewport for the RSM downsample */

	auto oldViewports = DX11Device::GetSingleton().GetViewports();

	D3D11_VIEWPORT viewport;

	viewport.Width    = m_sampleResolution;
	viewport.Height   = m_sampleResolution;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	DX11Device::GetSingleton().SetViewports(&viewport, 1);

	/* RSM Downsampling */

	DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);
	DX11Device::GetSingleton().SetBlending(false);

	ID3D11RenderTargetView * rsmRenderTargets[] = {
		m_position.GetRenderTargetView(),
		m_normal.GetRenderTargetView(),
		m_flux.GetRenderTargetView(),
		m_depth.GetRenderTargetView()
	};

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(4, rsmRenderTargets, nullptr);

	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(__MYE_DX11_SAMPLER_SLOT_LINEAR, 1, &m_linearSampler);

	float quad[] = {
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
	};

	DX11VertexBuffer quadBuffer;
	quadBuffer.Create(quad, 6, VertexDeclaration({ VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT4) }));

	m_lpvRSMSamplerVS->Use();
	m_lpvRSMSamplerPS->Use();

	Light * light = rsm.GetLight();

	MakeLightBuffer(m_lightBuffer, light);

	m_lightBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);

	rsm.GetPositionShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
	rsm.GetNormalShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER,   __MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
	rsm.GetFluxShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER,     __MYE_DX11_TEXTURE_SLOT_RSMFLUX);
	rsm.GetDepthShaderResource().Bind(DX11PipelineStage::PIXEL_SHADER,    __MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

	quadBuffer.Bind();

	DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11Device::GetSingleton().GetImmediateContext()->Draw(6, 0);

	quadBuffer.Unbind();

	rsm.GetPositionShaderResource().Unbind();
	rsm.GetNormalShaderResource().Unbind();
	rsm.GetFluxShaderResource().Unbind();
	rsm.GetDepthShaderResource().Unbind();

	DX11Module::GetSingleton().RenderShaderResource(m_flux, Vector2i(150, 0), Vector2i(150));

	/* Injection */

	m_lightVolume[0].SetRenderTarget();

	m_lpvInjectFluxVS->Use();
	m_lpvInjectFluxGS->Use();
	m_lpvInjectFluxPS->Use();

	m_position.Bind(DX11PipelineStage::VERTEX_SHADER, __MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
	m_normal.Bind(DX11PipelineStage::VERTEX_SHADER,   __MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
	m_flux.Bind(DX11PipelineStage::VERTEX_SHADER,     __MYE_DX11_TEXTURE_SLOT_RSMFLUX);

	viewport.Height   = m_volumeResolution;
	viewport.Width    = m_volumeResolution;

	DX11Device::GetSingleton().SetViewports(&viewport, 1);

	BindConfigurationBuffer(DX11PipelineStage::VERTEX_SHADER, 0);

	DX11Device::GetSingleton().GetImmediateContext()->OMSetBlendState(m_additiveBlend, nullptr, 0xFFFFFFFF);

	unsigned int numSamples = m_sampleResolution * m_sampleResolution;

	// No buffers bound, vertices are generated through SV_VertexID
	DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	DX11Device::GetSingleton().GetImmediateContext()->Draw(numSamples, 0);

	m_geometryVolume.SetRenderTarget();

	m_lpvInjectGeometryVS->Use();
	m_lpvInjectGeometryGS->Use();
	m_lpvInjectGeometryPS->Use();

	DX11Device::GetSingleton().GetImmediateContext()->Draw(numSamples, 0);

	m_position.Unbind();
	m_normal.Unbind();
	m_flux.Unbind();

	m_lpvInjectGeometryVS->Dispose();
	m_lpvInjectGeometryGS->Dispose();
	m_lpvInjectGeometryPS->Dispose();

	quadBuffer.Destroy();

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

	m_lpvPropagateVS->Use();
	m_lpvPropagateGS->Use();
	m_lpvPropagatePS->Use();

	unsigned int verticesCount = m_volumeResolution * m_volumeResolution * m_volumeResolution;

	m_geometryVolume.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_LPVGEOMETRY);
	BindConfigurationBuffer(DX11PipelineStage::VERTEX_SHADER, 0);

	for (unsigned int i = 0; i < iterations; i++)
	{

		uint8_t nextVolume = (i + 1) % 2;

		m_lightVolume[nextVolume].Clear();
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

	m_lpvPropagateVS->Dispose();
	m_lpvPropagateGS->Dispose();
	m_lpvPropagatePS->Dispose();

	DX11Device::GetSingleton().SetViewports(&oldViewports[0], oldViewports.size());

}

bool DX11LightPropagationVolume::__CreateShaders(void)
{

	VertexDeclaration lpvVD({
		VertexAttribute(VertexAttributeSemantic::POSITION,  DataFormat::FLOAT4),
	});

	auto lpvILV = MakeInputElementVector(lpvVD);

	m_lpvRSMSamplerVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/lpv_rsmsampler_vs.cso",
		nullptr,
		Parameters({
			{ "type", "vertex" },
			{ "precompiled", "true" },
			{ "inputLayoutVector", PointerToString(&lpvILV) }
		})
	);

	m_lpvRSMSamplerPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/lpv_rsmsampler_ps.cso",
		nullptr,
		Parameters({
			{ "type", "pixel" },
			{ "precompiled", "true" }
		})
	);

	m_lpvInjectFluxVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/lpv_injectflux_vs.cso",
		nullptr,
		Parameters({
			{ "type", "vertex" },
			{ "precompiled", "true" }
		})
	);

	m_lpvInjectFluxGS = ResourceTypeManager::GetSingleton().CreateResource<DX11GeometryShader>(
		"DX11Shader",
		"./shaders/lpv_injectflux_gs.cso",
		nullptr,
		Parameters({
			{ "type", "geometry" },
			{ "precompiled", "true" }
		})
	);

	m_lpvInjectFluxPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/lpv_injectflux_ps.cso",
		nullptr,
		Parameters({
			{ "type", "pixel" },
			{ "precompiled", "true" }
		})
	);

	m_lpvInjectGeometryVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/lpv_injectgeometry_vs.cso",
		nullptr,
		Parameters({
			{ "type", "vertex" },
			{ "precompiled", "true" }
		})
	);

	m_lpvInjectGeometryGS = ResourceTypeManager::GetSingleton().CreateResource<DX11GeometryShader>(
		"DX11Shader",
		"./shaders/lpv_injectgeometry_gs.cso",
		nullptr,
		Parameters({
			{ "type", "geometry" },
			{ "precompiled", "true" }
		})
	);

	m_lpvInjectGeometryPS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/lpv_injectgeometry_ps.cso",
		nullptr,
		Parameters({
			{ "type", "pixel" },
			{ "precompiled", "true" }
		})
	);

	m_lpvPropagateVS = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>(
		"DX11Shader",
		"./shaders/lpv_propagate_vs.cso",
		nullptr,
		Parameters({
			{ "type", "vertex" },
			{ "precompiled", "true" }
		})
	);

	m_lpvPropagateGS = ResourceTypeManager::GetSingleton().CreateResource<DX11GeometryShader>(
		"DX11Shader",
		"./shaders/lpv_propagate_gs.cso",
		nullptr,
		Parameters({
			{ "type", "geometry" },
			{ "precompiled", "true" }
		})
	);

	m_lpvPropagatePS = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>(
		"DX11Shader",
		"./shaders/lpv_propagate_ps.cso",
		nullptr,
		Parameters({
			{ "type", "pixel" },
			{ "precompiled", "true" }
		})
	);

	return m_lpvRSMSamplerVS->Load() &&
	       m_lpvRSMSamplerPS->Load() &&
	       m_lpvInjectFluxVS->Load() &&
	       m_lpvInjectFluxGS->Load() &&
	       m_lpvInjectFluxPS->Load() &&
	       m_lpvInjectGeometryVS->Load() &&
	       m_lpvInjectGeometryGS->Load() &&
	       m_lpvInjectGeometryPS->Load() &&
	       m_lpvPropagateVS->Load() &&
		   m_lpvPropagateGS->Load() &&
		   m_lpvPropagatePS->Load();

}

void DX11LightPropagationVolume::__DestroyShaders(void)
{

	m_lpvRSMSamplerVS->Unload();
	m_lpvRSMSamplerPS->Unload();

	m_lpvInjectFluxVS->Unload();
	m_lpvInjectFluxGS->Unload();
	m_lpvInjectFluxPS->Unload();

}

bool DX11LightPropagationVolume::__CreateBuffers(void)
{
	return m_lightBuffer.Create(sizeof(detail::LightBuffer)) &&
	       m_cameraTransformBuffer.Create(sizeof(Matrix4)) &&
		   m_lpvConfig.Create(sizeof(detail::LPVConfiguration));
}

void DX11LightPropagationVolume::__DestroyBuffers(void)
{
	m_lightBuffer.Destroy();
	m_cameraTransformBuffer.Destroy();
	m_lpvConfig.Destroy();
}

bool DX11LightPropagationVolume::__CreateContextStates(void)
{

	D3D11_SAMPLER_DESC linearSamplerDesc;

	linearSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	linearSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.MipLODBias     = 0.0f;
	linearSamplerDesc.MaxAnisotropy  = 0;
	linearSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	linearSamplerDesc.BorderColor[0] = 0;
	linearSamplerDesc.BorderColor[1] = 0;
	linearSamplerDesc.BorderColor[2] = 0;
	linearSamplerDesc.BorderColor[3] = 0;
	linearSamplerDesc.MinLOD         = 0;
	linearSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

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

	return !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&linearSamplerDesc, &m_linearSampler)) &&
	       !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateBlendState(&additiveBlendDesc, &m_additiveBlend));

}

void DX11LightPropagationVolume::__DestroyContextStates(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_linearSampler);
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

	return m_shR.Create(resolution, resolution, DataFormat::FLOAT4) &&
	       m_shG.Create(resolution, resolution, DataFormat::FLOAT4) &&
	       m_shB.Create(resolution, resolution, DataFormat::FLOAT4);

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

	return m_geometryVolume.Create(resolution, resolution, DataFormat::FLOAT4);

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