#include "DX11PixelShader.h"
#include "DX11Utils.h"

#include <d3dcompiler.h>

#include <mye/core/FileInputStream.h>
#include <mye/core/FileInfo.h>
#include <mye/core/Logger.h>

using namespace mye::dx11;
using namespace mye::core;

DX11PixelShader::DX11PixelShader(ResourceManager      * owner,
								 const String         & name,
								 ManualResourceLoader * manual,
								 bool precompiled) :
	DX11Shader(owner, name, manual, precompiled)
{
	m_shader = nullptr;
}


DX11PixelShader::~DX11PixelShader(void)
{
}

void DX11PixelShader::Use(void)
{
	DX11Device::GetSingleton().GetImmediateContext()->PSSetShader(m_shader, nullptr, 0);
}

void DX11PixelShader::Dispose(void)
{
	DX11Device::GetSingleton().GetImmediateContext()->PSSetShader(nullptr, nullptr, 0);
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

	ID3DBlob * code = nullptr;
	ID3DBlob * error = nullptr;

	UINT compileFlags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

#ifdef MYE_DX11_SHADERS_DEBUG
	compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	String sourceFile;

	if (m_params.Contains("source"))
	{
		sourceFile = m_params.GetString("source");
	}
	else
	{
		sourceFile = m_name;
	}

	if (m_precompiled)
	{

		std::wstring wfilename;
		wfilename.assign(sourceFile.begin(), sourceFile.end());

		D3DReadFileToBlob(wfilename.c_str(), &code);

	}
	else
	{

		auto defines = CreateDefinesVector();

		defines.insert(defines.begin(), { "MYE_PIXEL_SHADER", "" });

		String sourceCode = LoadSourceCode();

		if (__MYE_DX11_HR_TEST_FAILED(D3DCompile(
				sourceCode.CString(),
				sourceCode.Length(),
				sourceFile.CString(),
				&defines[0],
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main",
				"ps_5_0",
				compileFlags,
				0x0,
				&code,
				&error)))
		{
			m_compileError = (LPCSTR) error->GetBufferPointer();
			Logger::LogErrorOptional("DX11 Shader Compilation", m_compileError);
		}

		// Need to remove statically allocated strings before deleting
		defines.erase(defines.begin());

		FreeDefinesVector(defines);

	}

	if (code &&
		!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->
			CreatePixelShader(code->GetBufferPointer(),
			                  code->GetBufferSize(),
			                  nullptr,
			                  &m_shader)))
	{
		success = true;
	}
	else
	{
		Logger::LogErrorOptional("DX11 Shader Compilation", sourceFile + " does not appear to be a valid compiled pixel shader.");
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