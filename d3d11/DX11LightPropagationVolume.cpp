#include "DX11LightPropagationVolume.h"
#include "DX11VertexBuffer.h"
#include "DX11Defines.h"

#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include "DX11Module.h"

#include "detail/ShadersBuffers.h"

using namespace mye::core;
using namespace mye::dx11;
using namespace mye::dx11::detail;
using namespace mye::math;

DX11LightPropagationVolume::DX11LightPropagationVolume(void) { }

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

	m_geometryVolume.SetParametersList(volumeParams);

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

	return	m_position.Create(resolution, resolution, DataFormat::FLOAT4) &&
	        m_normal.Create(resolution, resolution, DataFormat::FLOAT4) &&
	        m_flux.Create(resolution, resolution, DataFormat::FLOAT4) &&
	        m_depth.Create(resolution, resolution, DataFormat::FLOAT) &&
			m_lightVolume[0].Create(dimensions) &&
			m_lightVolume[1].Create(dimensions) &&
			m_geometryVolume.Create(dimensions, dimensions, DataFormat::FLOAT4) &&
			m_lightBuffer.Create(sizeof(detail::LightBuffer)) &&
			m_cameraTransformBuffer.Create(sizeof(Matrix4)) &&
	        m_lpvRSMSamplerVS->Load() &&
	        m_lpvRSMSamplerPS->Load() &&
	        !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&linearSamplerDesc, &m_linearSampler));

}

void DX11LightPropagationVolume::Destroy(void)
{

	m_position.Destroy();
	m_normal.Destroy();
	m_flux.Destroy();

	m_lightVolume[0].Destroy();
	m_lightVolume[1].Destroy();

	m_geometryVolume.Destroy();

	m_lpvRSMSamplerVS->Unload();
	m_lpvRSMSamplerPS->Unload();

}

void DX11LightPropagationVolume::Init(const Camera * camera, const Vector3 & volumeMinCorner, const Vector3 & volumeMaxCorner)
{
	
	Matrix4 viewProjection = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	m_cameraTransformBuffer.SetData(viewProjection.Data());

	m_lightVolume[0].Clear();

	Vector3 diffCorner = volumeMaxCorner - volumeMinCorner;

	m_volumeMinCorner  = volumeMinCorner;
	m_volumeMaxCorner  = volumeMaxCorner;

	m_volumeCellSize   = Max(diffCorner.x(), diffCorner.y(), diffCorner.z());
	

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

	ID3D11RenderTargetView * renderTargets[] = {
		m_position.GetRenderTargetView(),
		m_normal.GetRenderTargetView(),
		m_flux.GetRenderTargetView(),
		m_depth.GetRenderTargetView()
	};

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(4, renderTargets, nullptr);

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

	detail::LightBuffer lightBuffer;

	const Matrix4 & t = light->GetWorldMatrix();

	lightBuffer.color     = mye::math::Vector4f(light->GetColor(), 1);
	lightBuffer.position  = t * mye::math::Vector4f(light->GetPosition(), 1);
	lightBuffer.direction = t * mye::math::Vector4f(light->GetDirection(), 1);
	lightBuffer.intensity = light->GetIntensity();
	lightBuffer.spotAngle = light->GetSpotAngle();
	lightBuffer.range     = light->GetRange();

	m_lightBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_BUFFER_SLOT_LIGHT);
	m_lightBuffer.SetData(&lightBuffer);

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
	DX11Module::GetSingleton().RenderShaderResource(rsm.GetNormalShaderResource(), Vector2i(300, 0), Vector2i(150));

	/* Injection */

	m_position.Bind(DX11PipelineStage::VERTEX_SHADER, __MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
	m_normal.Bind(DX11PipelineStage::VERTEX_SHADER,   __MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
	m_flux.Bind(DX11PipelineStage::VERTEX_SHADER,     __MYE_DX11_TEXTURE_SLOT_RSMFLUX);

	viewport.Height   = m_volumeResolution;
	viewport.Width    = m_volumeResolution;

	DX11Device::GetSingleton().SetViewports(&viewport, 1);

	m_lightVolume[0].SetRenderTarget();

	// No buffers bound, vertices are generated through SV_VertexID
	DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	DX11Device::GetSingleton().GetImmediateContext()->Draw(m_sampleResolution * m_sampleResolution, 0);

	m_position.Unbind();
	m_normal.Unbind();
	m_flux.Unbind();

	DX11Device::GetSingleton().SetViewports(&oldViewports[0], oldViewports.size());
	
	quadBuffer.Destroy();

}

void DX11LightPropagationVolume::Propagate(size_t iterations)
{

}

/* LightVolume */

LightVolume::LightVolume(void) { }

bool LightVolume::Create(size_t resolution)
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

void LightVolume::Destroy(void)
{
	m_shR.Destroy();
	m_shG.Destroy();
	m_shB.Destroy();
}

void LightVolume::SetRenderTarget(void)
{

	ID3D11RenderTargetView * rt[] = {
		m_shR.GetRenderTargetView(),
		m_shG.GetRenderTargetView(),
		m_shB.GetRenderTargetView()
	};

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(3, rt, nullptr);

}

void LightVolume::Bind(DX11PipelineStage stage, int slot)
{
	m_shR.Bind(stage, slot);
	m_shG.Bind(stage, slot);
	m_shB.Bind(stage, slot);
}

void LightVolume::Unbind(void)
{
	m_shR.Unbind();
	m_shG.Unbind();
	m_shB.Unbind();
}

void LightVolume::Clear(const Vector4f & color)
{
	m_shR.ClearRenderTarget(color);
	m_shG.ClearRenderTarget(color);
	m_shB.ClearRenderTarget(color);
}