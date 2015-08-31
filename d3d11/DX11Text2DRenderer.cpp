#include "DX11Text2DRenderer.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include "DX11Font.h"
#include "DX11VertexBuffer.h"

#include "./detail/ShadersBuffers.h"

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

DX11Text2DRenderer::DX11Text2DRenderer(void) :
	m_initialized(false)
{
}


DX11Text2DRenderer::~DX11Text2DRenderer(void)
{
}

bool DX11Text2DRenderer::Init(void)
{

	D3D11_SAMPLER_DESC fontSamplerDesc;

	ZeroMemory(&fontSamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	fontSamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	fontSamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
	fontSamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
	fontSamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
	fontSamplerDesc.MipLODBias     = 0.0f;
	fontSamplerDesc.MaxAnisotropy  = 1;
	fontSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	fontSamplerDesc.MinLOD         = 0.0f;
	fontSamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&fontSamplerDesc, &m_fontTextureSampler)))
	{


		VertexDeclaration textVD({
			VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT2),
			VertexAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2)
		});

		auto textILV = MakeInputElementVector(textVD);

		m_text2d = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
			"DX11Shader",
			"./shaders/text2d.msh",
			nullptr,
			{ { "type", "program" } });

		if (m_text2d->Load() &&
		    __CreateConstantBuffers())
		{

			m_initialized = true;

		}

	}

	return m_initialized;

}

void DX11Text2DRenderer::Shutdown(void)
{

	if (m_initialized)
	{

		__MYE_DX11_RELEASE_COM(m_fontTextureSampler);

		m_text2d->Unload();

		m_initialized = false;

	}

}

void DX11Text2DRenderer::Render(ID3D11RenderTargetView * target)
{

	m_text2d->Use();

	DX11Device::GetSingleton().GetImmediateContext()->OMSetRenderTargets(1, &target, nullptr);
	DX11Device::GetSingleton().GetImmediateContext()->PSSetSamplers(0, 1, &m_fontTextureSampler);

	DX11Device::GetSingleton().SetBlending(true);
	DX11Device::GetSingleton().SetDepthTest(DX11DepthTest::OFF);

	SceneModule * scene = Game::GetSingleton().GetSceneModule();

	for (Text2DComponent * t2dc : scene->GetText2DList())
	{

		// Draw text
		
		DX11FontPointer         font = Resource::DynamicCast<DX11Font>(t2dc->GetFont());
		DX11VertexBufferPointer vb   = Resource::DynamicCast<DX11VertexBuffer>(t2dc->GetVertexBuffer());

		if (vb && font && font->Load() && vb->Load())
		{

			font->Bind();
			vb->Bind();

			m_textColorBuffer.SetData(t2dc->GetColor().Data());
			m_textColorBuffer.Bind(DX11PipelineStage::PIXEL_SHADER, 0);

			DX11Device::GetSingleton().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DX11Device::GetSingleton().GetImmediateContext()->Draw(vb->GetVerticesCount(), 0);

			vb->Unbind();
			font->Unbind();

		}

	}

}

bool DX11Text2DRenderer::__CreateConstantBuffers(void)
{

	return m_textColorBuffer.Create(sizeof(mye::math::Vector4f));

}