#include "EntityParser.h"

#include "GameObject.h"
#include "Game.h"

#include <fstream>
#include <initializer_list>

#include <boost/variant/static_visitor.hpp>

#include <mye/lua/LuaModule.h>

using namespace mye::core;

qi::rule<std::string::iterator, std::string()> PrimitiveParser::s_quotedString;

struct EntityMembersVisitor : public boost::static_visitor<bool>
{

	EntityMembersVisitor(Entity &templ) : templ(templ) { }

	bool operator() (const DataMember &m) const;
	bool operator() (const ComponentMember &m) const;

	mye::core::Entity &templ;

};

EntityParser::EntityParser(const String &file)
{

	std::ifstream f(file.CString());

	if (f)
	{
		m_code = std::string(
			std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>());
	}

}

EntityParser::~EntityParser(void)
{
}

bool EntityParser::LexicalParse(ParsedEntity &entity) const
{

	EntityDefinition<Iterator> entityGrammar;

	Iterator it = m_code.begin();
	Iterator end = m_code.end();

	bool r = qi::phrase_parse(it, end, entityGrammar, EntitySkipGrammar<Iterator>(), entity);

	return (r && it == end);

}

bool EntityParser::SemanticParse(ParsedEntity &entity, Entity &templ) const
{

	//std::vector<std::string> v;

	//v.push_back("1337");
	//v.push_back("sup mang");
	//v.push_back("3.14");

	//ComponentMember test;

	//test.component   = ComponentTypes::TEXT2D;
	//test.initializer = v;

	//PrimitiveParser::ParseComponentInitializer<int, std::string, float>(test);

	if (templ.GetName() == entity.name.c_str())
	{

		//for (DataMember &m : entity.variables)

		EntityMembersVisitor visitor(templ);

		for (auto &m : entity.members)
		{

			if (!boost::apply_visitor(visitor, m))
			{
				return false;
			}

		}

	}

	return true;

}

bool EntityMembersVisitor::operator() (const DataMember &m) const
{

	bool parsed = true;

	switch (m.data)
	{

	case DataTypes::STRING:

		{

			std::string s;

			qi::rule<EntityParser::Iterator, std::string()> quoted_string;

			quoted_string %=
				qi::skip['"'] >>
				*(qi::char_ - '"') >>
				qi::skip['"'];


			if (m.initializer.size() == 1)
			{

				auto it = m.initializer[0].begin();
				auto end = m.initializer[0].end();

				if (!qi::parse(it, end, quoted_string, s) ||
					it != end)
				{
					parsed = false;
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
			break;

		}

		

	case DataTypes::BOOL:

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
				parsed = false;
			}

			VariableComponent<bool> *component =
				new VariableComponent<bool>(m.name.c_str());
			component->Set(x);

			templ.Insert(component);

			break;

		}

	case DataTypes::INT:

		{

			int x;

			if (m.initializer.size() == 1)
			{

				auto it = m.initializer[0].begin();
				auto end = m.initializer[0].end();

				if (!qi::parse(it, end, qi::int_, x) ||
					it != end)
				{
					parsed = false;
				}

			}
			else if (m.initializer.size() != 0)
			{
				parsed = false;
			}

			VariableComponent<int> *component =
				new VariableComponent<int>(m.name.c_str());
			component->Set(x);

			templ.Insert(component);

			break;

		}

	case DataTypes::REAL:

		{

			float x;

			if (m.initializer.size() == 1)
			{

				auto it = m.initializer[0].begin();
				auto end = m.initializer[0].end();

				if (!qi::parse(it, end, qi::float_, x) ||
					it != end)
				{
					parsed = false;
				}

			}
			else if (m.initializer.size() != 0)
			{
				parsed = false;
			}

			VariableComponent<float> *component =
				new VariableComponent<float>(m.name.c_str());
			component->Set(x);

			templ.Insert(component);

			break;

		}

	case DataTypes::VEC3:

		{

			mye::math::Vector3 v;

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
						parsed = false;
					}

					v[i] = x;

				}

			}
			else if (m.initializer.size() != 0)
			{
				parsed = false;
			}

			VariableComponent<mye::math::Vector3> *component =
				new VariableComponent<mye::math::Vector3>(m.name.c_str());
			component->Set(v);

			templ.Insert(component);

			break;

		}

	case DataTypes::VEC3I:

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
						parsed = false;
					}

					v[i] = x;

				}

			}
			else if (m.initializer.size() != 0)
			{
				parsed = false;
			}

			VariableComponent<mye::math::Vector3i> *component =
				new VariableComponent<mye::math::Vector3i>(m.name.c_str());
			component->Set(v);

			templ.Insert(component);

			break;

		}

	case DataTypes::VEC4:

		{

			mye::math::Vector4 v;

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
						parsed = false;
					}

					v[i] = x;

				}

			}
			else if (m.initializer.size() != 0)
			{
				parsed = false;
			}

			VariableComponent<mye::math::Vector4> *component =
				new VariableComponent<mye::math::Vector4>(m.name.c_str());
			component->Set(v);

			templ.Insert(component);

			break;

		}

	case DataTypes::VEC4I:

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
						parsed = false;
					}

					v[i] = x;

				}

			}
			else if (m.initializer.size() != 0)
			{
				parsed = false;
			}

			VariableComponent<mye::math::Vector4i> *component =
				new VariableComponent<mye::math::Vector4i>(m.name.c_str());
			component->Set(v);

			templ.Insert(component);

			break;

		}

	case DataTypes::QUATERNION:

		{

			mye::math::Quaternion q;
			mye::math::Vector4 in;

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
						parsed = false;
					}

					in[i] = x;

				}

				q = mye::math::Quaternion(in[0], in[1], in[2], in[3]);

			}
			else if (m.initializer.size() != 0)
			{
				parsed = false;
			}

			VariableComponent<mye::math::Quaternion> *component =
				new VariableComponent<mye::math::Quaternion>(m.name.c_str());
			component->Set(q);

			templ.Insert(component);

			break;

		}

	case DataTypes::GAMEOBJECTHANDLE:

		{

			if (m.initializer.size() != 0)
			{
				parsed = false;
			}

			VariableComponent<GameObjectHandle> *component =
				new VariableComponent<GameObjectHandle>(m.name.c_str());

			templ.Insert(component);

			break;

		}

	}

	return parsed;

}

#define __MYE_ENTITY_MEMBERS_VISITOR(__COMPONENT, __COMPONENTTYPE) case ComponentTypes::__COMPONENTTYPE:\
{ auto c = Game::GetSingleton().GetScriptModule()->Create<__COMPONENT>(#__COMPONENT, m.initializer.c_str()); if (c) templ.Insert(c.get().Clone()); break; }

bool EntityMembersVisitor::operator() (const ComponentMember &m) const
{
	
	switch (m.component)
	{

		__MYE_ENTITY_MEMBERS_VISITOR(Text2DComponent,    TEXT2D)
		__MYE_ENTITY_MEMBERS_VISITOR(RenderComponent,    RENDER)
		__MYE_ENTITY_MEMBERS_VISITOR(CameraComponent,    CAMERA)
		__MYE_ENTITY_MEMBERS_VISITOR(RigidBodyComponent, RIGIDBODY)
		__MYE_ENTITY_MEMBERS_VISITOR(TransformComponent, TRANSFORM)
		__MYE_ENTITY_MEMBERS_VISITOR(LightComponent,     LIGHT)

	}

	return true;
}