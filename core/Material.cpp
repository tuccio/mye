#include "Material.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <fstream>

#define __MYE_MATERIAL_SET_DEFAULT() { m_diffuseColor = Vector4(1.0f); m_specularColor = Vector4(1.0f); m_roughness = 0.5f; m_specular = 0.0f; m_metallic = 0.0f; }

using namespace mye::core;
using namespace mye::math;

Material::Material(ResourceManager * owner,
				   const String & name,
				   ManualResourceLoader * manual) :
	Resource(owner, name, manual)
{
	__MYE_MATERIAL_SET_DEFAULT();
}

bool Material::LoadImpl(void)
{
	
	std::ifstream in(m_name.CString());

	if (in.is_open())
	{

		try
		{

			using boost::property_tree::ptree;

			ptree pt;

			read_json(in, pt);

			auto diffuse_color = pt.get_child_optional("diffuse_color");
			auto specular_color = pt.get_child_optional("specular_color");

			auto specular  = pt.get_optional<float>("specular");
			auto roughness = pt.get_optional<float>("roughness");
			auto metallic  = pt.get_optional<float>("metallic");

			__MYE_MATERIAL_SET_DEFAULT();


			if (diffuse_color)
			{

				int i = 0;
				for (auto & n : *diffuse_color)
				{
					m_diffuseColor[i++] = n.second.get_value<Real>();
				}

			}

			if (specular_color)
			{

				int i = 0;
				for (auto & n : *specular_color)
				{
					m_specularColor[i++] = n.second.get_value<Real>();
				}

			}

			if (specular)
			{
				m_specular = *specular;
			}

			if (roughness)
			{
				m_roughness = *roughness;
			}

			if (metallic)
			{
				m_metallic = *metallic;
			}

			return true;

		}
		catch (...) { }

	}

	return false;

}

void Material::UnloadImpl(void)
{
	__MYE_MATERIAL_SET_DEFAULT();
}

size_t Material::CalculateSizeImpl(void)
{
	return sizeof(Material);
}