#include "DX11SSAO.h"

#include <mye/core/ResourceTypeManager.h>

using namespace mye::core;
using namespace mye::dx11;

DX11SSAO::DX11SSAO(void) :
m_width(0),
m_height(0)
{
}

bool DX11SSAO::Create(int width, int height)
{

	m_width = width;
	m_height = height;

	m_ssao = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/ssao.msh",
		nullptr,
		{ { "type", "program" } }
	);

	m_ssaoBlur = ResourceTypeManager::GetSingleton().CreateResource<DX11ShaderProgram>(
		"DX11Shader",
		"./shaders/ssao_blur.msh",
		nullptr,
		{ { "type", "program" } }
	);

	return m_ssao->Load() &&
	       m_ssaoBlur->Load() &&
	       m_ssaoBuffer.Create(width, height, DataFormat::HALF2) &&
	       m_blurBuffer.Create(width, height, DataFormat::HALF2);

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

}