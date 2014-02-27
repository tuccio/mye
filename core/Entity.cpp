#include "Entity.h"
#include "GameObject.h"
#include "ColorRGBA.h"
#include "ColorRGB.h"

#include <fstream>

using namespace mye::core;
using namespace mye::core::entity;

Parser::Parser(const String &file)
{

	std::ifstream f(file.CString());

	if (f)
	{
		m_code = std::string(
			std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>());
	}

}


Parser::~Parser(void)
{
}

bool Parser::SemanticParse(Entity &entity, EntityTemplate &templ) const
{

	if (templ.GetName() == entity.name.c_str())
	{

		for (Member &m : entity.members)
		{

			switch (m.type)
			{

			case TypeEnum::STRING:

				{

					std::string s;

					qi::rule<std::string::iterator, std::string()> quoted_string;

					quoted_string %=
						qi::skip['"'] >>
						* (qi::char_ - '"') >>
						qi::skip['"'];


					if (m.initializer.size() == 1)
					{

						auto it = m.initializer[0].begin();
						auto end = m.initializer[0].end();

						if (!qi::parse(it, end, quoted_string, s) ||
							it != end)
						{
							return false;
						}

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<String> *component =
						new VariableComponent<String>(m.name.c_str());
					component->Set(s.c_str());

					templ.Insert(component);

				}

				break;

			case TypeEnum::BOOL:

				{

					bool x;

					if (m.initializer.size() == 1)
					{

						auto it = m.initializer[0].begin();
						auto end = m.initializer[0].end();

						if (!qi::parse(it, end, qi::bool_, x) ||
							it != end)
						{
							return false;
						}

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<bool> *component =
						new VariableComponent<bool>(m.name.c_str());
					component->Set(x);

					templ.Insert(component);

					break;

				}

			case TypeEnum::INT:

				{

					int x;

					if (m.initializer.size() == 1)
					{

						auto it = m.initializer[0].begin();
						auto end = m.initializer[0].end();

						if (!qi::parse(it, end, qi::int_, x) ||
							it != end)
						{
							return false;
						}

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<int> *component =
						new VariableComponent<int>(m.name.c_str());
					component->Set(x);

					templ.Insert(component);

					break;

				}

			case TypeEnum::FLOAT:

				{

					float x;

					if (m.initializer.size() == 1)
					{

						auto it = m.initializer[0].begin();
						auto end = m.initializer[0].end();

						if (!qi::parse(it, end, qi::float_, x) ||
							it != end)
						{
							return false;
						}

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<float> *component =
						new VariableComponent<float>(m.name.c_str());
					component->Set(x);

					templ.Insert(component);

					break;

				}

			case TypeEnum::VEC3:

				{

					mye::math::Vector3f v;

					if (m.initializer.size() == 3)
					{

						float x;

						for (int i = 0; i < 3; i++)
						{

							auto it = m.initializer[i].begin();
							auto end = m.initializer[i].end();

							if (!qi::parse(it, end, qi::float_, x) ||
								it != end)
							{
								return false;
							}

							v[i] = x;

						}

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<mye::math::Vector3f> *component =
						new VariableComponent<mye::math::Vector3f>(m.name.c_str());
					component->Set(v);

					templ.Insert(component);

					break;

				}

			case TypeEnum::VEC3I:

				{

					mye::math::Vector3i v;

					if (m.initializer.size() == 3)
					{

						int x;

						for (int i = 0; i < 3; i++)
						{

							auto it = m.initializer[i].begin();
							auto end = m.initializer[i].end();

							if (!qi::parse(it, end, qi::int_, x) ||
								it != end)
							{
								return false;
							}

							v[i] = x;

						}

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<mye::math::Vector3i> *component =
						new VariableComponent<mye::math::Vector3i>(m.name.c_str());
					component->Set(v);

					templ.Insert(component);

					break;

				}

			case TypeEnum::VEC4:

				{

					mye::math::Vector4f v;

					if (m.initializer.size() == 4)
					{

						float x;

						for (int i = 0; i < 4; i++)
						{

							auto it = m.initializer[i].begin();
							auto end = m.initializer[i].end();

							if (!qi::parse(it, end, qi::float_, x) ||
								it != end)
							{
								return false;
							}

							v[i] = x;

						}

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<mye::math::Vector4f> *component =
						new VariableComponent<mye::math::Vector4f>(m.name.c_str());
					component->Set(v);

					templ.Insert(component);

					break;

				}

			case TypeEnum::VEC4I:

				{

					mye::math::Vector4i v;

					if (m.initializer.size() == 4)
					{

						int x;

						for (int i = 0; i < 4; i++)
						{

							auto it = m.initializer[i].begin();
							auto end = m.initializer[i].end();

							if (!qi::parse(it, end, qi::int_, x) ||
								it != end)
							{
								return false;
							}

							v[i] = x;

						}

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<mye::math::Vector4i> *component =
						new VariableComponent<mye::math::Vector4i>(m.name.c_str());
					component->Set(v);

					templ.Insert(component);

					break;

				}

			case TypeEnum::QUATERNION:

				{

					mye::math::Quaternionf q;
					mye::math::Vector4f in;

					if (m.initializer.size() == 4)
					{

						float x;

						for (int i = 0; i < 4; i++)
						{

							auto it = m.initializer[i].begin();
							auto end = m.initializer[i].end();

							if (!qi::parse(it, end, qi::float_, x) ||
								it != end)
							{
								return false;
							}

							in[i] = x;

						}

						q = mye::math::Quaternionf(in[0], in[1], in[2], in[3]);

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<mye::math::Quaternionf> *component =
						new VariableComponent<mye::math::Quaternionf>(m.name.c_str());
					component->Set(q);

					templ.Insert(component);

					break;

				}

			case TypeEnum::RGB:

				{

					ColorRGB c;
					mye::math::Vector3f in;

					if (m.initializer.size() == 3)
					{

						float x;

						for (int i = 0; i < 3; i++)
						{

							auto it = m.initializer[i].begin();
							auto end = m.initializer[i].end();

							if (!qi::parse(it, end, qi::float_, x) ||
								it != end)
							{
								return false;
							}

							in[i] = x;

						}

						c = ColorRGB(in[0], in[1], in[2]);

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<ColorRGB> *component =
						new VariableComponent<ColorRGB>(m.name.c_str());
					component->Set(c);

					templ.Insert(component);

					break;

				}

			case TypeEnum::RGBA:

				{

					ColorRGBA c;
					mye::math::Vector4f in;

					if (m.initializer.size() == 4)
					{

						float x;

						for (int i = 0; i < 4; i++)
						{

							auto it = m.initializer[i].begin();
							auto end = m.initializer[i].end();

							if (!qi::parse(it, end, qi::float_, x) ||
								it != end)
							{
								return false;
							}

							in[i] = x;

						}

						c = ColorRGBA(in[0], in[1], in[2], in[3]);

					}
					else if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<ColorRGBA> *component =
						new VariableComponent<ColorRGBA>(m.name.c_str());
					component->Set(c);

					templ.Insert(component);

					break;

				}

			case TypeEnum::GAMEOBJECTHANDLE:

				{

					if (m.initializer.size() != 0)
					{
						return false;
					}

					VariableComponent<GameObjectHandle> *component =
						new VariableComponent<GameObjectHandle>(m.name.c_str());

					templ.Insert(component);

					break;

				}

			}

		}

		return true;

	}

	return false;

}

EntityTemplate::EntityTemplate(ResourceManager *owner,
				 const String &name,
				 ManualResourceLoader *manual,
				 const String &entityDirectory) :
	Resource(owner, name, manual),
	m_entityDirectory(entityDirectory)
{

}

void EntityTemplate::Insert(Component *component)
{

	m_components.push_back(component);

}

void EntityTemplate::Clear(void)
{

	for (Component *c : m_components)
	{
		delete c;
	}

	m_components.clear();

}

bool EntityTemplate::LoadImpl(void)
{
	
	bool loaded = false;

	String file = m_entityDirectory + m_name + ".mye";

	Parser p(file);

	Entity e;

	if (p.LexicalParse(e) && p.SemanticParse(e, *this))
	{
		loaded = true;
	}

	return loaded;

}

void EntityTemplate::UnloadImpl(void)
{

	Clear();

}

size_t EntityTemplate::CalculateSizeImpl(void)
{

	size_t size = 0;

	for (Component *c : m_components)
	{
		size += sizeof(*c);
	}

	return size;

}

EntityTemplateManager::EntityTemplateManager(const String &entityDirectory) :
	ResourceManager("Entity"),
	m_entityDirectory(entityDirectory)
{
}

ResourceHandle EntityTemplateManager::CreateImpl(const String &name,
												 ManualResourceLoader *manual,
												 Resource::ParametersList *params)
{
	return ResourceHandle(new EntityTemplate(this, name, manual, m_entityDirectory));
}

void EntityTemplateManager::FreeImpl(Resource* resource)
{
	static_cast<EntityTemplate*>(resource)->Clear();
}