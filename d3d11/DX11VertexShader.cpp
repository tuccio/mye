#include "DX11VertexShader.h"
#include "DX11Utils.h"

#include <d3dcompiler.h>

using namespace mye::dx11;

DX11VertexShader::DX11VertexShader(mye::core::ResourceManager *owner,
								   const std::string &name,
								   mye::core::ManualResourceLoader *manual,
								   DX11Device &device) :
DX11Shader(owner, name, manual),
	m_device(device)
{
	m_shader = NULL;
}


DX11VertexShader::~DX11VertexShader(void)
{
}

ID3D11VertexShader* DX11VertexShader::GetVertexShader(void)
{
	return m_shader;
}

std::string DX11VertexShader::GetCompileError(void)
{
	return m_compileError;
}


bool DX11VertexShader::LoadImpl(void)
{

	bool success = false;

	if (DX11Shader::LoadImpl())
	{
		
		ID3DBlob *code, *error;
		UINT compileFlags = 0x0;

#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		
		if (FAILED(D3DCompile(
			m_source.c_str(),
			m_source.length(),
			m_name.c_str(),
			NULL,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"vs_5_0",
			compileFlags,
			0x0,
			&code,
			&error)))
		{
			m_compileError = (LPCSTR) error->GetBufferPointer();
		}
		else if (!FAILED(m_device.GetDevice()->CreateVertexShader(
			code->GetBufferPointer(),
			code->GetBufferSize(),
			NULL,
			&m_shader)))
		{
			success = true;
		}

		DX11Shader::UnloadImpl();

	}

	return success;

}

void DX11VertexShader::UnloadImpl(void)
{

	if (m_shader)
	{
		ReleaseCOM(m_shader);
	}

	m_compileError.clear();

}

size_t DX11VertexShader::CalculateSizeImpl(void)
{
	return 0;
}