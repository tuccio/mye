#include "DX11VertexShader.h"
#include "DX11Utils.h"

#include <d3dcompiler.h>

#include <mye/core/FileInputStream.h>
#include <mye/core/FileInfo.h>
#include <mye/core/Utils.h>

#include <sstream>
#include <string>

using namespace mye::dx11;
using namespace mye::core;

DX11VertexShader::DX11VertexShader(mye::core::ResourceManager * owner,
								   const mye::core::String & name,
								   mye::core::ManualResourceLoader * manual,
								   DX11Device & device,
								   bool precompiled) :
DX11Shader(owner, name, manual, precompiled),
	m_device(device)
{
	m_shader      = nullptr;
	m_inputLayout = nullptr;
}

void DX11VertexShader::Use(void)
{
	m_device.GetImmediateContext()->IASetInputLayout(m_inputLayout);
	m_device.GetImmediateContext()->VSSetShader(m_shader, nullptr, 0);
}

void DX11VertexShader::Dispose(void)
{
	m_device.GetImmediateContext()->GSSetShader(nullptr, nullptr, 0);
}


DX11VertexShader::~DX11VertexShader(void)
{
}

ID3D11VertexShader * DX11VertexShader::GetVertexShader(void)
{
	return m_shader;
}

mye::core::String DX11VertexShader::GetCompileError(void)
{
	return m_compileError;
}


bool DX11VertexShader::LoadImpl(void)
{

	bool success = false;

	if (DX11Shader::LoadImpl())
	{
		
		ID3DBlob * code = nullptr;
		ID3DBlob * error = nullptr;

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
				"vs_5_0",
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
				CreateVertexShader(
					code->GetBufferPointer(),
					code->GetBufferSize(),
					nullptr,
					&m_shader)))
		{

			if (m_params.Contains("inputLayoutVector"))
			{

				const void * ptr = m_params.GetPointer("inputLayoutVector");

				const std::vector<D3D11_INPUT_ELEMENT_DESC> * vDesc = static_cast<const std::vector<D3D11_INPUT_ELEMENT_DESC>*>(ptr);

				if (!__MYE_DX11_HR_TEST_FAILED(m_device.GetDevice()->
					CreateInputLayout(
						&vDesc->front(),
						vDesc->size(),
						code->GetBufferPointer(),
						code->GetBufferSize(),
						&m_inputLayout)))
				{
					success = true;
				}

			}
			else
			{
				success = true;
			}

		}

	}

	if (!success)
	{
		Destroy();
	}

	return success;

}

void DX11VertexShader::UnloadImpl(void)
{
	Destroy();
}

size_t DX11VertexShader::CalculateSizeImpl(void)
{
	return 0;
}

void DX11VertexShader::Destroy(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shader);
	m_compileError.Clear();
}