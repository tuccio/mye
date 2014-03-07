#include "DX11Shader.h"

#include <d3d11.h>
#include <fstream>

using namespace mye::dx11;
using namespace mye::core;

DX11Shader::DX11Shader(ResourceManager *owner,
					   const mye::core::String &name,
					   ManualResourceLoader *manual,
					   bool precompiled) :
Resource(owner, name, manual),
	m_precompiled(precompiled)
{
}

DX11Shader::~DX11Shader(void)
{

}

void DX11Shader::Destroy(void)
{
	m_source.Clear();
}

void DX11Shader::Use(void)
{

}

bool DX11Shader::LoadImpl(void)
{

	bool success = false;
	std::ifstream f(m_name.CString());
	
	if (f)
	{

		if (!m_precompiled)
		{

			m_source = std::string(std::istreambuf_iterator<char>(f),
				std::istreambuf_iterator<char>()).c_str();

		}
		
		success = true;
		f.close();
		
	}

	return success;
	
}

void DX11Shader::UnloadImpl(void)
{
	Destroy();
}

size_t DX11Shader::CalculateSizeImpl(void)
{
	return m_source.Length();
}