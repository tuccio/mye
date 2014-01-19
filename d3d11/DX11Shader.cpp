#include "DX11Shader.h"

#include <d3d11.h>
#include <fstream>

using namespace mye::dx11;
using namespace mye::core;

DX11Shader::DX11Shader(ResourceManager *owner,
					   const std::string &name,
					   ManualResourceLoader *manual) :
Resource(owner, name, manual)
{

}

DX11Shader::~DX11Shader(void)
{

}

void DX11Shader::Destroy(void)
{
	m_source.clear();
}

void DX11Shader::Use(void)
{

}

bool DX11Shader::LoadImpl(void)
{

	std::ifstream f(m_name);
	
	if (f)
	{
		m_source = std::string(std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>());
		return true;
	}

	return false;
	
}

void DX11Shader::UnloadImpl(void)
{
	Destroy();
}

size_t DX11Shader::CalculateSizeImpl(void)
{
	return m_source.length();
}