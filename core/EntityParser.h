#pragma once

#include "Entity.h"
#include "Components.h"

#include <vector>
#include <string>
#include <tuple>

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <boost/preprocessor/repeat.hpp>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/variant.hpp>

namespace mye
{

	namespace core
	{

		namespace qi = boost::spirit::qi;
		namespace ascii = boost::spirit::ascii;
		namespace phoenix = boost::phoenix;

		enum DataTypes
		{
			STRING,
			BOOL,
			INT,
			FLOAT,
			VEC3,
			VEC3I,
			VEC4,
			VEC4I,
			QUATERNION,
			GAMEOBJECTHANDLE
		};

		struct DataMember
		{
			DataTypes data;
			std::string name;
			std::vector<std::string> initializer;
		};

		struct ComponentMember
		{
			ComponentTypes component;
			std::string initializer;
		};

		/*struct ParsedEntity
		{
			std::string name;
			std::vector<DataMember> variables;
			std::vector<ComponentMember> components;
		};*/

		typedef boost::variant<DataMember, ComponentMember> EntityMembers;

		struct ParsedEntity
		{
			std::string name;
			std::vector<EntityMembers> members;
		}; 

		struct DataTypeSymbols : qi::symbols<char, DataTypes>
		{

			DataTypeSymbols(void)
			{

				add
					("string",           DataTypes::STRING)
					("bool",             DataTypes::BOOL)
					("int",              DataTypes::INT)
					("float",            DataTypes::FLOAT)
					("vec3",             DataTypes::VEC3)
					("vec3i",            DataTypes::VEC3I)
					("vec4",             DataTypes::VEC4)
					("vec4i",            DataTypes::VEC4I)
					("quaternion",       DataTypes::QUATERNION)
					("GameObjectHandle", DataTypes::GAMEOBJECTHANDLE);

			}

		};

		struct ComponentSymbols : qi::symbols<char, ComponentTypes>
		{

			ComponentSymbols(void)
			{

				add
					("TransformComponent", ComponentTypes::TRANSFORM)
					("RenderComponent",    ComponentTypes::RENDER)
					("CameraComponent",    ComponentTypes::CAMERA)
					("RigidBodyComponent", ComponentTypes::RIGIDBODY)
					("Text2DComponent",    ComponentTypes::TEXT2D);

			}

		};

		template <typename Iterator>
		struct EntitySkipGrammar : qi::grammar<Iterator>
		{

			EntitySkipGrammar(void);

			qi::rule<Iterator> start;
			qi::rule<Iterator> commentLine;
			qi::rule<Iterator> commentMultiline;

		};

		template <typename Iterator>
		struct EntityDefinition : qi::grammar<Iterator, ParsedEntity(), EntitySkipGrammar<Iterator>>
		{

			EntityDefinition(void);

			qi::rule<Iterator, ParsedEntity(), EntitySkipGrammar<Iterator>> start;

			qi::rule<Iterator, std::string()> identifier;

			qi::rule<Iterator, std::vector<std::string>(), EntitySkipGrammar<Iterator>> dataMemberInitializer;

			qi::rule<Iterator, DataMember(), EntitySkipGrammar<Iterator>> dataMember;
			qi::rule<Iterator, EntityMembers(), EntitySkipGrammar<Iterator>> dataMemberVar;

			qi::rule<Iterator, std::string(), EntitySkipGrammar<Iterator>> componentMemberConstructor;
			qi::rule<Iterator, std::string(), EntitySkipGrammar<Iterator>> componentMemberArguments;

			qi::rule<Iterator, ComponentMember(), EntitySkipGrammar<Iterator>> componentMember;
			qi::rule<Iterator, EntityMembers(), EntitySkipGrammar<Iterator>> componentMemberVar;

			DataTypeSymbols dataType;
			ComponentSymbols componentType;

		};

		enum PrimitiveDataTypes
		{
			ENTITY_INT,
			ENTITY_UINT,
			ENTITY_FLOAT,
			ENTITY_DOUBLE,
			ENTITY_BOOL,
			ENTITY_STRING
		};

// 		typedef std::vector<PrimitiveDataTypes> EntityMemberInitializer;
// 		typedef std::vector<EntityMemberInitializer> EntityMemberInitalizersList;

		template <typename ... Types>
		struct EntityMemberInitializer
		{

			std::tuple<Types ...> Parse(const std::vector<std::string> &initializer);

		};

		class PrimitiveParser
		{

		public:

			static int ParseInt(const std::string &s);
			static unsigned int ParseUnsignedInt(const std::string &s);

			static float ParseFloat(const std::string &s);
			static double ParseDouble(const std::string &s);

			static bool ParseBool(const std::string &s);
			static String ParseString(const std::string &s);

			template <typename ... Types>
			static std::tuple<Types ...> ParseComponentInitializer(ComponentMember &m);

		private:

			static qi::rule<std::string::iterator, std::string()> s_quotedString;

		};

		class EntityParser
		{

		public:

			typedef std::string::const_iterator Iterator;

			EntityParser(const String &file);
			~EntityParser(void);

			bool LexicalParse(ParsedEntity &entity) const;
			bool SemanticParse(ParsedEntity &entity, mye::core::Entity &templ) const;

		private:

			std::string m_code;

		};

	}

}

#include "EntityParser.inl"