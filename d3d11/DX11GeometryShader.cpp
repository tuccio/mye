#include "DX11GeometryShader.h"
#include "DX11Utils.h"

#include <d3dcompiler.h>

#include <mye/core/FileInputStream.h>
#include <mye/core/FileInfo.h>
#include <mye/core/Logger.h>
#include <mye/core/Utils.h>

#include <algorithm>
#include <sstream>
#include <string>

using namespace mye::dx11;
using namespace mye::core;

DX11GeometryShader::DX11GeometryShader(ResourceManager      * owner,
                                       const String         & name,
                                       ManualResourceLoader * manual,
                                       bool precompiled) :
	DX11Shader(owner, name, manual, precompiled)
{
	m_shader = nullptr;
}

void DX11GeometryShader::Use(void)
{
	DX11Device::GetSingleton().GetImmediateContext()->GSSetShader(m_shader, nullptr, 0);
}

void DX11GeometryShader::Dispose(void)
{
	DX11Device::GetSingleton().GetImmediateContext()->GSSetShader(nullptr, nullptr, 0);
}

DX11GeometryShader::~DX11GeometryShader(void)
{
}

ID3D11GeometryShader * DX11GeometryShader::GetGeometryShader(void)
{
	return m_shader;
}

mye::core::String DX11GeometryShader::GetCompileError(void)
{
	return m_compileError;
}


bool DX11GeometryShader::LoadImpl(void)
{

	bool success = false;

	ID3DBlob * code = nullptr;
	ID3DBlob * error = nullptr;

	UINT compileFlags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

#ifdef _DEBUG
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

		defines.insert(defines.begin(), { "MYE_GEOMETRY_SHADER", "" });

		String sourceCode = LoadSourceCode();

		if (__MYE_DX11_HR_TEST_FAILED(D3DCompile(
			sourceCode.CString(),
			sourceCode.Length(),
			sourceFile.CString(),
			&defines[0],
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"gs_5_0",
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
			CreateGeometryShader(
				code->GetBufferPointer(),
				code->GetBufferSize(),
				nullptr,
				&m_shader)))
	{
		success = true;
	}
	else
	{
		Logger::LogErrorOptional("DX11 Shader Compilation", sourceFile + " does not appear to be a valid compiled geometry shader.");
	}

	if (!success)
	{
		Destroy();
	}

	return success;

}

void DX11GeometryShader::UnloadImpl(void)
{
	Destroy();
}

size_t DX11GeometryShader::CalculateSizeImpl(void)
{
	return 0;
}

void DX11GeometryShader::Destroy(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shader);
}