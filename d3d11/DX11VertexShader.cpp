#include "DX11VertexShader.h"
#include "DX11Utils.h"

#include <d3dcompiler.h>

#include <mye/core/Logger.h>
#include <mye/core/FileInputStream.h>
#include <mye/core/FileInfo.h>
#include <mye/core/Utils.h>

#include <sstream>
#include <string>

#include <boost/property_tree/json_parser.hpp>

#include <boost/algorithm/string.hpp>

using namespace mye::dx11;
using namespace mye::core;

static VertexDeclaration __ParseJSONInput(const String & input);

DX11VertexShader::DX11VertexShader(ResourceManager      * owner,
                                   const String         & name,
                                   ManualResourceLoader * manual,
                                   bool                   precompiled) :
	DX11Shader(owner, name, manual, precompiled)
{
	m_shader      = nullptr;
	m_inputLayout = nullptr;
}

void DX11VertexShader::Use(void)
{
	DX11Device::GetSingleton().GetImmediateContext()->IASetInputLayout(m_inputLayout);
	DX11Device::GetSingleton().GetImmediateContext()->VSSetShader(m_shader, nullptr, 0);
}

void DX11VertexShader::Dispose(void)
{
	DX11Device::GetSingleton().GetImmediateContext()->VSSetShader(nullptr, nullptr, 0);
}


DX11VertexShader::~DX11VertexShader(void)
{
}

ID3D11VertexShader * DX11VertexShader::GetVertexShader(void)
{
	return m_shader;
}

const String & DX11VertexShader::GetCompileError(void)
{
	return m_compileError;
}

bool DX11VertexShader::LoadImpl(void)
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

		defines.insert(defines.begin(), { "MYE_VERTEX_SHADER", "" });

		String sourceCode = LoadSourceCode();

		if (__MYE_DX11_HR_TEST_FAILED(D3DCompile(
			sourceCode.CString(),
			sourceCode.Length(),
			sourceFile.CString(),
			&defines[0],
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"vs_5_0",
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
			CreateVertexShader(code->GetBufferPointer(),
			                   code->GetBufferSize(),
			                   nullptr,
			                   &m_shader)))
	{

		if (m_params.Contains("input"))
		{

			String input = m_params.GetString("input");

			auto vDesc = MakeInputElementVector(__ParseJSONInput(input));

			if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->
				CreateInputLayout(&vDesc[0], vDesc.size(), code->GetBufferPointer(), code->GetBufferSize(), &m_inputLayout)))
			{
				success = true;
			}
			else
			{
				Logger::LogErrorOptional("DX11 Shader Compilation", "CreateInputLayout failed for vertex shader '" + sourceFile + "'.");
			}

		}
		else
		{
			success = true;
		}

	}
	else
	{
		Logger::LogErrorOptional("DX11 Shader Compilation", sourceFile + " does not appear to be a valid compiled vertex shader.");
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

static VertexDeclaration __ParseJSONInput(const String & input)
{

	std::istringstream ss;

	ss.str(input.CString());

	boost::property_tree::ptree pt;

	try
	{
		read_json(ss, pt);
	}
	catch (boost::property_tree::json_parser_error & e)	{ }

	VertexDeclaration vd;

	for (auto & arrayElement : pt.get_child("input"))
	{

		for (auto & p : arrayElement.second)
		{

			DataFormat format;
			VertexAttributeSemantic attribute;

			auto second = p.second.get_value<std::string>();

			if (ParseVertexAttributeSemantic(p.first.c_str(), attribute) &&
				ParseDataFormat(second.c_str(), format))
			{
				vd.AddAttribute(attribute, format);
			}

		}

	}

	return vd;

}