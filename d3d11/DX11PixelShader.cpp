#include "DX11PixelShader.h"
#include "DX11Utils.h"

#include <d3dcompiler.h>

using namespace mye::dx11;

DX11PixelShader::DX11PixelShader(mye::core::ResourceManager *owner,
								   const std::string &name,
								   mye::core::ManualResourceLoader *manual,
								   DX11Device &device) :
DX11Shader(owner, name, manual),
	m_device(device)
{
	m_shader = NULL;
}


DX11PixelShader::~DX11PixelShader(void)
{
}

ID3D11PixelShader* DX11PixelShader::GetPixelShader(void)
{
	return m_shader;
}

std::string DX11PixelShader::GetCompileError(void)
{
	return m_compileError;
}


bool DX11PixelShader::LoadImpl(void)
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
			"ps_5_0",
			compileFlags,
			0x0,
			&code,
			&error)))
		{
			m_compileError = (LPCSTR) error->GetBufferPointer();
		}
		else if (!FAILED(m_device.GetDevice()->CreatePixelShader(
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

void DX11PixelShader::UnloadImpl(void)
{

	if (m_shader)
	{
		ReleaseCOM(m_shader);
	}

	m_compileError.clear();

}

size_t DX11PixelShader::CalculateSizeImpl(void)
{
	return 0;
}