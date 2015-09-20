#include "DX11SSAO.h"
#include "DX11VertexBuffer.h"

#include "DX11Module.h"
	
#include <mye/core/ResourceTypeManager.h>

using namespace mye::core;
using namespace mye::dx11;
using namespace mye::math;

DX11SSAO::DX11SSAO(void) :
m_width(0),
m_height(0)
{
}

bool DX11SSAO::Create(int width, int height)
{

	m_width = width;
	m_height = height;

	m_ssaoBuffer.SetParametersList({ { "renderTarget", "true" } });
	m_blurBuffer.SetParametersList({ { "renderTarget", "true" } });

	m_ssao = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/ssao.msh",
		nullptr,
		{ { "type", "program" } }
	);

	String wResolution = ToString(width);
	String hResolution = ToString(height);

	m_ssaoBlur[0] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/ssao_blur_h.msh",
		nullptr,
		{ { "type", "program" } }
	);

	m_ssaoBlur[1] = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/ssao_blur_v.msh",
		nullptr,
		{ { "type", "program" } }
	);

	return m_ssao->Load() &&
	       m_ssaoBlur[0]->Load() &&
	       m_ssaoBlur[1]->Load() &&
	       m_ssaoBuffer.Create(width, height, DataFormat::HALF) &&
	       m_blurBuffer.Create(width, height, DataFormat::HALF);

}

DX11ShaderResource & DX11SSAO::GetSSAOBuffer(void)
{
	return m_ssaoBuffer;
}

int DX11SSAO::GetWidth(void) const
{
	return m_width;
}

int DX11SSAO::GetHeight(void) const
{
	return m_height;
}

void DX11SSAO::Render(void)
{
	
	auto viewports = DX11Device::GetSingleton().GetViewports();

	D3D11_VIEWPORT viewport;

	viewport.Height   = m_height;
	viewport.Width    = m_width;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	DX11Device::GetSingleton().SetViewports(&viewport, 1);

	auto ssaoBuffer = m_ssaoBuffer.GetRenderTargetView();
	auto blurBuffer = m_blurBuffer.GetRenderTargetView();

	DX11Device::GetSingleton().SetBlending(false);
	DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);
	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &ssaoBuffer, nullptr);

	m_ssao->Use();

	DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

	// Horizontal blur pass

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &blurBuffer, nullptr);

	m_ssaoBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_OCCLUSION);

	m_ssaoBlur[0]->Use();
	DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

	// Vertical blur pass

	m_ssaoBuffer.Unbind();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &ssaoBuffer, nullptr);

	m_blurBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, __MYE_DX11_TEXTURE_SLOT_OCCLUSION);

	m_ssaoBlur[1]->Use();
	DX11Device::GetSingleton().GetImmediateContext()->Draw(4, 0);

	m_blurBuffer.Unbind();

	DX11Module::GetSingleton().RenderShaderResource(m_ssaoBuffer, Vector2i(0), Vector2i(640, 360));

	DX11Device::GetSingleton().SetViewports(&viewports[0], viewports.size());
	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);

}

void DX11SSAO::Destroy(void)
{
	m_ssaoBuffer.Destroy();
	m_blurBuffer.Destroy();
}