#include "DX11PixelShader.h"
#include "DX11Utils.h"

#include <d3dcompiler.h>

#include <mye/core/FileInputStream.h>
#include <mye/core/FileInfo.h>

using namespace mye::dx11;
using namespace mye::core;

DX11PixelShader::DX11PixelShader(mye::core::ResourceManager * owner,
								   const mye::core::String & name,
								   mye::core::ManualResourceLoader * manual,
								   DX11Device & device,
								   bool precompiled) :
DX11Shader(owner, name, manual, precompiled),
	m_device(device)
{
	m_shader = nullptr;
}


DX11PixelShader::~DX11PixelShader(void)
{
}

void DX11PixelShader::Use(void)
{
	m_device.GetImmediateContext()->PSSetShader(m_shader, nullptr, 0);
}

ID3D11PixelShader * DX11PixelShader::GetPixelShader(void)
{
	return m_shader;
}

const mye::core::String & DX11PixelShader::GetCompileError(void)
{
	return m_compileError;
}


bool DX11PixelShader::LoadImpl(void)
{

	bool success = false;

	if (DX11Shader::LoadImpl())
	{

		ID3DBlob *code = nullptr;
		ID3DBlob *error = nullptr;

		UINT compileFlags = 0x0;

#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		if (m_precompiled)
		{

			std::wstring wfilename;
			wfilename.assign(m_name.begin(), m_name.end());

			D3DReadFileToBlob(wfilename.c_str(), &code);

		}
		else
		{

			if (__MYE_DX11_HR_TEST_FAILED(D3DCompile(
					m_source.CString(),
					m_source.Length(),
					m_name.CString(),
					nullptr,
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

		}

		if (code &&
			!__MYE_DX11_HR_TEST_FAILED(m_device.GetDevice()->
				CreatePixelShader(
					code->GetBufferPointer(),
					code->GetBufferSize(),
					nullptr,
					&m_shader)))
		{
			success = true;
		}
		

	}

	if (!success)
	{
		Destroy();
	}

	return success;

}

void DX11PixelShader::UnloadImpl(void)
{

	Destroy();

}

size_t DX11PixelShader::CalculateSizeImpl(void)
{
	return 0;
}

void DX11PixelShader::Destroy(void)
{
	
	if (m_shader)
	{
		__MYE_DX11_RELEASE_COM(m_shader);
	}
	
	m_compileError.Clear();

}