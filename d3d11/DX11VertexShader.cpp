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

			if (__MYE_DX11_HR_TEST_FAILED(D3DCompile(
				m_source.CString(),
				m_source.Length(),
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

			}
			else if (m_params.Contains("inputLayoutVector")) // TODO: Remove
			{

				const void * ptr = m_params.GetPointer("inputLayoutVector");

				const std::vector<D3D11_INPUT_ELEMENT_DESC> * vDesc = static_cast<const std::vector<D3D11_INPUT_ELEMENT_DESC>*>(ptr);

				if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->
					CreateInputLayout(&vDesc->front(),
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
		else
		{
			Logger::LogErrorOptional("DX11 Shader Compilation", sourceFile + " does not appear to be a valid compiled vertex shader.");
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

static VertexDeclaration __ParseJSONInput(const String & input)
{

	std::istringstream ss;

	ss.str(input.CString());

	boost::property_tree::ptree pt;

	try
	{
		read_json(ss, pt);
	}
	catch (boost::property_tree::json_parser_error &) { }

	VertexDeclaration vd;

	for (auto & p : pt)
	{

		DataFormat format;
		VertexAttributeSemantic semantic;

		if (boost::iequals("position", p.first))
		{
			semantic = VertexAttributeSemantic::POSITION;
		}
		else if (boost::iequals("normal", p.first))
		{
			semantic = VertexAttributeSemantic::NORMAL;
		}
		else if (boost::iequals("texcoord0", p.first))
		{
			semantic = VertexAttributeSemantic::TEXCOORD0;
		}
		else if (boost::iequals("texcoord1", p.first))
		{
			semantic = VertexAttributeSemantic::TEXCOORD1;
		}
		else if (boost::iequals("texcoord2", p.first))
		{
			semantic = VertexAttributeSemantic::TEXCOORD2;
		}
		else if (boost::iequals("texcoord3", p.first))
		{
			semantic = VertexAttributeSemantic::TEXCOORD3;
		}
		else if (boost::iequals("texcoord4", p.first))
		{
			semantic = VertexAttributeSemantic::TEXCOORD4;
		}
		else if (boost::iequals("texcoord5", p.first))
		{
			semantic = VertexAttributeSemantic::TEXCOORD5;
		}
		else if (boost::iequals("texcoord6", p.first))
		{
			semantic = VertexAttributeSemantic::TEXCOORD6;
		}
		else if (boost::iequals("texcoord7", p.first))
		{
			semantic = VertexAttributeSemantic::TEXCOORD7;
		}
		else if (boost::iequals("tangent", p.first))
		{
			semantic = VertexAttributeSemantic::TANGENT;
		}
		else if (boost::iequals("bitangent", p.first))
		{
			semantic = VertexAttributeSemantic::BITANGENT;
		}
		else if (boost::iequals("diffuse", p.first))
		{
			semantic = VertexAttributeSemantic::DIFFUSE;
		}
		else if (boost::iequals("specular", p.first))
		{
			semantic = VertexAttributeSemantic::SPECULAR;
		}
		else
		{
			continue;
		}

		auto second = p.second.get_value<std::string>();

		if (boost::iequals("float", second))
		{
			format = DataFormat::FLOAT;
		}
		else if (boost::iequals("float2", second))
		{
			format = DataFormat::FLOAT2;
		}
		else if (boost::iequals("float3", second))
		{
			format = DataFormat::FLOAT3;
		}
		else if (boost::iequals("float4", second))
		{
			format = DataFormat::FLOAT4;
		}
		else if (boost::iequals("int", second))
		{
			format = DataFormat::INT;
		}
		else if (boost::iequals("int2", second))
		{
			format = DataFormat::INT2;
		}
		else if (boost::iequals("int3", second))
		{
			format = DataFormat::INT3;
		}
		else if (boost::iequals("int4", second))
		{
			format = DataFormat::INT4;
		}
		else
		{
			continue;
		}

		vd.AddAttribute(semantic, format);

	}

	return vd;

}