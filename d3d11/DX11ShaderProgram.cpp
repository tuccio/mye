#include "DX11ShaderProgram.h"

#include <mye/core/Logger.h>
#include <mye/core/ResourceTypeManager.h>

#include <boost/property_tree/json_parser.hpp>

#include <sstream>

using namespace mye::core;
using namespace mye::dx11;

DX11ShaderProgram::DX11ShaderProgram(ResourceManager * owner, const String & name, ManualResourceLoader * manual) :
DX11Shader(owner, name, manual) { }

bool DX11ShaderProgram::LoadImpl(void)
{

	using namespace boost::property_tree;

	ptree ptCommonDefines;

	if (m_params.Contains("defines"))
	{

		std::istringstream ss(m_params.GetString("defines").CString());

		try
		{
			json_parser::read_json(ss, ptCommonDefines);
		}
		catch (json_parser_error &) { }

	}

	ptree pt;

	try
	{
		json_parser::read_json(m_name.CString(), pt);
	}
	catch (json_parser_error & e)
	{
		Logger::LogErrorOptional("DX11ShaderProgram Parse Error", e.what());
		return false;
	}

	auto vertexShader   = pt.get_child_optional("vertex");
	auto pixelShader    = pt.get_child_optional("pixel");
	auto geometryShader = pt.get_child_optional("geometry");

	if (vertexShader)
	{

		auto name        = vertexShader->get_optional<std::string>("name");
		auto source      = vertexShader->get_optional<std::string>("source");
		auto precompiled = vertexShader->get_optional<bool>("precompiled");
		auto input       = vertexShader->get_child_optional("input");
		auto defines     = vertexShader->get_child_optional("defines");

		ptree ptDefines = ptCommonDefines;

		if (defines)
		{
			for (auto & d : *defines)
			{
				ptDefines.insert(ptDefines.begin(), d);
			}
		}

		if (source)
		{

			Parameters params;

			params.Add("type", "vertex");
			params.Add("source", source->c_str());

			{
				std::stringstream ss;
				json_parser::write_json(ss, ptDefines);
				params.Add("defines", ss.str().c_str());
			}

			if (precompiled && *precompiled)
			{
				params.Add("precompiled", "true");
			}

			if (input)
			{

				ptree ptInput;
				ptInput.add_child("input", *input);

				std::stringstream ss;

				json_parser::write_json(ss, ptInput);
				params.Add("input", ss.str().c_str());

			}

			String resourceName = (name ? name->c_str() : source->c_str());

			m_vertexShader = ResourceTypeManager::GetSingleton().CreateResource<DX11VertexShader>("DX11Shader", resourceName, nullptr, params);

			if (!m_vertexShader->Load())
			{
				return false;
			}

		}
		

	}

	if (pixelShader)
	{

		auto name        = pixelShader->get_optional<std::string>("name");
		auto source      = pixelShader->get_optional<std::string>("source");
		auto precompiled = pixelShader->get_optional<bool>("precompiled");
		auto defines     = pixelShader->get_child_optional("defines");

		ptree ptDefines = ptCommonDefines;

		if (defines)
		{
			for (auto & d : *defines)
			{
				ptDefines.insert(ptDefines.begin(), d);
			}
		}

		if (source)
		{

			Parameters params;

			params.Add("type", "pixel");
			params.Add("source", source->c_str());

			{
				std::stringstream ss;
				json_parser::write_json(ss, ptDefines);
				params.Add("defines", ss.str().c_str());
			}

			if (precompiled && *precompiled)
			{
				params.Add("precompiled", "true");
			}

			String resourceName = (name ? name->c_str() : source->c_str());

			m_pixelShader = ResourceTypeManager::GetSingleton().CreateResource<DX11PixelShader>("DX11Shader", resourceName, nullptr, params);

			if (!m_pixelShader->Load())
			{
				return false;
			}

		}
		

	}

	if (geometryShader)
	{

		auto name        = geometryShader->get_optional<std::string>("name");
		auto source      = geometryShader->get_optional<std::string>("source");
		auto precompiled = geometryShader->get_optional<bool>("precompiled");
		auto defines     = geometryShader->get_child_optional("defines");

		ptree ptDefines = ptCommonDefines;

		if (defines)
		{
			for (auto & d : *defines)
			{
				ptDefines.insert(ptDefines.begin(), d);
			}
		}

		if (source)
		{

			Parameters params;

			params.Add("type", "geometry");
			params.Add("source", source->c_str());

			{
				std::stringstream ss;
				json_parser::write_json(ss, ptDefines);
				params.Add("defines", ss.str().c_str());
			}

			if (precompiled && *precompiled)
			{
				params.Add("precompiled", "true");
			
			}
			
			String resourceName = (name ? name->c_str() : source->c_str());

			m_geometryShader = ResourceTypeManager::GetSingleton().CreateResource<DX11GeometryShader>("DX11Shader", resourceName, nullptr, params);

			if (!m_geometryShader->Load())
			{
				return false;
			}

		}


	}

	return true;

}

size_t DX11ShaderProgram::CalculateSizeImpl(void)
{

	size_t size = 0;

	if (m_vertexShader)
	{
		size += m_vertexShader->GetSize();
	}

	if (m_geometryShader)
	{
		size += m_geometryShader->GetSize();
	}

	if (m_pixelShader)
	{
		size += m_pixelShader->GetSize();
	}
	

	return size;
}

void DX11ShaderProgram::UnloadImpl(void)
{

	m_vertexShader   = nullptr;
	m_pixelShader    = nullptr;
	m_geometryShader = nullptr;

}

void DX11ShaderProgram::Use(void)
{

	if (m_vertexShader)
	{
		m_vertexShader->Use();
	}

	if (m_pixelShader)
	{
		m_pixelShader->Use();
	}

	if (m_geometryShader)
	{
		m_geometryShader->Use();
	}

}

void DX11ShaderProgram::Dispose(void)
{

	if (m_vertexShader)
	{
		m_vertexShader->Dispose();
	}

	if (m_pixelShader)
	{
		m_pixelShader->Dispose();
	}

	if (m_geometryShader)
	{
		m_geometryShader->Dispose();
	}

}

void DX11ShaderProgram::Destroy(void)
{

	if (m_vertexShader)
	{
		m_vertexShader->Destroy();
	}

	if (m_pixelShader)
	{
		m_pixelShader->Destroy();
	}

	if (m_geometryShader)
	{
		m_geometryShader->Destroy();
	}

}