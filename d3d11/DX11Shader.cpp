#include "DX11Shader.h"

#include <mye/core/Logger.h>

#include <fstream>
#include <sstream>

#include <boost/property_tree/json_parser.hpp>
#include <cstring>

using namespace mye::dx11;
using namespace mye::core;

DX11Shader::DX11Shader(ResourceManager         * owner,
					   const mye::core::String & name,
					   ManualResourceLoader    * manual,
					   bool precompiled) :
	Resource(owner, name, manual),
	m_precompiled(precompiled) { }

DX11Shader::~DX11Shader(void) { }

void DX11Shader::Destroy(void)
{
}

void DX11Shader::Use(void)
{

}

bool DX11Shader::LoadImpl(void)
{
	return false;
}

void DX11Shader::UnloadImpl(void)
{
}

size_t DX11Shader::CalculateSizeImpl(void)
{
	return 0;
}

std::vector<D3D_SHADER_MACRO> DX11Shader::CreateDefinesVector(void) const
{

	std::vector<D3D_SHADER_MACRO> defines;

	if (m_params.Contains("defines"))
	{

		std::istringstream ss;

		ss.str(m_params.GetString("defines").CString());

		boost::property_tree::ptree pt;

		try
		{
			read_json(ss, pt);
		}
		catch (boost::property_tree::json_parser_error &) { }

		for (auto & define : pt)
		{

			auto second = define.second.get_value<std::string>();

			char * name       = new char[define.first.size() + 1];
			char * definition = new char[second.size() + 1];

			memcpy(name,       define.first.c_str(), define.first.size() + 1);
			memcpy(definition, second.c_str(),       second.size() + 1);

			D3D_SHADER_MACRO shaderMacro;
			
			shaderMacro.Name       = name;
			shaderMacro.Definition = definition;

			defines.push_back(shaderMacro);

		}

	}

	defines.push_back({ nullptr, nullptr });

	return defines;

}

void DX11Shader::FreeDefinesVector(std::vector<D3D_SHADER_MACRO> & defines) const
{

	for (auto & define : defines)
	{

		if (define.Name)
		{
			delete const_cast<char*>(define.Name);
		}

		if (define.Definition)
		{
			delete const_cast<char*>(define.Definition);
		}

	}

	defines.clear();

}

String DX11Shader::LoadSourceCode(void)
{

	String sourceCode;
	String sourceFile;

	if (m_params.Contains("source"))
	{
		sourceFile = m_params.GetString("source");
	}
	else
	{
		sourceFile = m_name;
	}

	std::ifstream f(sourceFile.CString());

	if (f)
	{

		sourceCode = std::string(std::istreambuf_iterator<char>(f),
		                         std::istreambuf_iterator<char>()).c_str();

	}
	else
	{

		Logger::LogErrorOptional("DX11 Shader Compilation", "Can't open file " + sourceFile);

	}

	return sourceCode;

}