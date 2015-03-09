#pragma once

#include "Components.h"
#include "VariableTypes.h"

#include <vector>
#include <string>
#include <tuple>

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_confix.hpp>
#include <boost/spirit/include/classic_chset.hpp>

#include <boost/preprocessor/repeat.hpp>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/variant.hpp>

namespace mye
{

	namespace core
	{

		namespace entity
		{

			namespace      qi = boost::spirit::qi;
			namespace   ascii = boost::spirit::ascii;
			namespace phoenix = boost::phoenix;
			

			/* Skip grammar */

			template <typename Iterator>
			struct SkipGrammar : qi::grammar<Iterator>
			{

				SkipGrammar(void) :
					base_type(start)
				{

					commentLine %= qi::lit("//")  >> *(qi::char_ - qi::eol - qi::eoi) >> (qi::eol | qi::eoi);
					commentMultiline %= qi::lit("/*") >> *(qi::char_ - "*/") >> qi::lit("*/");
					start %= ascii::space | commentLine | commentMultiline;

				}

				qi::rule<Iterator> start;
				qi::rule<Iterator> commentLine;
				qi::rule<Iterator> commentMultiline;

			};

			/* Identifiers */

			template <typename Iterator>
			struct Identifier : qi::grammar<Iterator, std::string()>
			{

				Identifier(void) : base_type(identifier)
				{

					identifier %=
						qi::lexeme[(qi::alpha | qi::char_('_')) >>
							* (qi::alnum | qi::char_('_'))];

				}

				qi::rule<Iterator, std::string()> identifier;

			};

			/* Quoted string */

			template <typename Iterator>
			struct QuotedString : qi::grammar<Iterator, std::string(), SkipGrammar<Iterator>>
			{

				QuotedString(void) : base_type(quotedString)
				{

					escapedChars.add
						("\\n",  '\n')
						("\\t",  '\t')
						("\\\"", '"')
						("\\\\", '\\')
						("\\'",  '\'');

					quotedString %=

						qi::lexeme [

							(qi::lit("\"") >>
							*(escapedChars | (qi::char_ - '"')) >>
							qi::lit("\"")) |

							(qi::lit("'") >>
							*(escapedChars | (qi::char_ - '\'')) >>
							qi::lit("'"))

						];

				}

				qi::rule<Iterator, std::string(), SkipGrammar<Iterator>> quotedString;
				qi::symbols<char, char> escapedChars;

			};

			/* Parameters */

			template <typename Iterator>
			struct ParametersGrammar : qi::grammar<Iterator, Parameters(), SkipGrammar<Iterator>>
			{

				ParametersGrammar(void) : base_type(parameters)
				{

					parameters = qi::omit['{'] >>
						- ( ( quotedString >> qi::omit[':'] >> quotedString ) [ phoenix::bind(&Parameters::Add, qi::_val, phoenix::bind(&std::string::c_str, qi::_1), phoenix::bind(&std::string::c_str, qi::_2)) ] % ',' ) >>
						qi::omit['}'];

				}

				qi::rule<Iterator, Parameters(), SkipGrammar<Iterator>> parameters;
				QuotedString<Iterator> quotedString;

			};

			/* Variables */

			enum class Constructible
			{
				IMPLICIT,
				STRING,
				BOOL,
				INT,
				REAL,
				VEC2,
				VEC2I,
				VEC3,
				VEC3I,
				VEC4,
				VEC4I,
				MAT2,
				MAT3,
				MAT4,
				QUATERNION,
				GAMEOBJECTHANDLE,
				PARAMETERS,
				MESH,
				MATERIAL,
				COLLISIONSHAPE,
				NIL,
			};

			struct InitValue
			{

				typedef boost::variant<bool, int, double, std::nullptr_t, std::string, Parameters> ValueType;

				Constructible constructible;
				std::vector<InitValue> initializers;
				ValueType value;

			};

			template <typename Iterator>
			struct ExplicitInitializerParser :
				qi::grammar<Iterator, InitValue(), SkipGrammar<Iterator>>
			{
				
				ExplicitInitializerParser(void) : base_type(initializer)
				{

					constructible.add
						("string",         Constructible::STRING)
						("bool",           Constructible::BOOL)
						("int",            Constructible::INT)
						("real",           Constructible::REAL)
						("vec2",           Constructible::VEC2)
						("vec2i",          Constructible::VEC2I)
						("vec3",           Constructible::VEC3)
						("vec3i",          Constructible::VEC3I)
						("vec4",           Constructible::VEC4)
						("vec4i",          Constructible::VEC4I)
						("mat2",           Constructible::MAT2)
						("mat3",           Constructible::MAT3)
						("mat4",           Constructible::MAT4)
						("quaternion",     Constructible::QUATERNION)
						("GameObject",     Constructible::GAMEOBJECTHANDLE)
						("Mesh",           Constructible::MESH)
						("Material",       Constructible::MATERIAL)
						("CollisionShape", Constructible::COLLISIONSHAPE);

					primitive =

						quotedString    [ phoenix::at_c<0>(qi::_val) = Constructible::STRING,     phoenix::at_c<2>(qi::_val) = qi::_1  ] |
						strictDouble    [ phoenix::at_c<0>(qi::_val) = Constructible::REAL,       phoenix::at_c<2>(qi::_val) = qi::_1  ] |
						qi::int_        [ phoenix::at_c<0>(qi::_val) = Constructible::INT,        phoenix::at_c<2>(qi::_val) = qi::_1  ] |
						qi::bool_       [ phoenix::at_c<0>(qi::_val) = Constructible::BOOL,       phoenix::at_c<2>(qi::_val) = qi::_1  ] |
						parameters      [ phoenix::at_c<0>(qi::_val) = Constructible::PARAMETERS, phoenix::at_c<2>(qi::_val) = qi::_1  ] |
						qi::lit("null") [ phoenix::at_c<0>(qi::_val) = Constructible::NIL,        phoenix::at_c<2>(qi::_val) = nullptr ];

					constructor =

						constructible [ phoenix::at_c<0>(qi::_val) = qi::_1 ] >> qi::omit['('] >>
						- ( ( primitive | constructor ) [ phoenix::push_back(phoenix::at_c<1>(qi::_val), qi::_1) ] % ',' ) >>
						qi::omit[')'];

					initializer %= primitive | constructor;

				}

				qi::rule<Iterator, InitValue(), SkipGrammar<Iterator>> initializer;
				qi::rule<Iterator, InitValue(), SkipGrammar<Iterator>> primitive;
				qi::rule<Iterator, InitValue(), SkipGrammar<Iterator>> constructor;

				qi::real_parser<double, qi::strict_real_policies<double>> strictDouble;

				qi::symbols<char, Constructible> constructible;

				QuotedString<Iterator> quotedString;
				ParametersGrammar<Iterator> parameters;

			};

			template <typename Iterator>
			struct ImplicitInitializerParser :
				qi::grammar<Iterator, InitValue(), SkipGrammar<Iterator>>
			{

				ImplicitInitializerParser(void) : base_type(initializer)
				{

					initializer =
						( qi::omit['('] >>
						( explicitInitializer [ phoenix::push_back(phoenix::at_c<1>(qi::_val), qi::_1) ] % ',' ) >>
						qi::omit[')']) [ phoenix::at_c<0>(qi::_val) = Constructible::IMPLICIT ];

				}

				qi::rule<Iterator, InitValue(), SkipGrammar<Iterator>> initializer;
				ExplicitInitializerParser<Iterator> explicitInitializer;

			};

			/* Symbols */

			struct DataTypeSymbols : qi::symbols<char, VariableTypes>
			{

				DataTypeSymbols(void)
				{

					add
						("string",     VariableTypes::STRING)
						("bool",       VariableTypes::BOOL)
						("int",        VariableTypes::INT)
						("real",       VariableTypes::REAL)
						("vec2",       VariableTypes::VEC2)
						("vec2i",      VariableTypes::VEC2I)
						("vec3",       VariableTypes::VEC3)
						("vec3i",      VariableTypes::VEC3I)
						("vec4",       VariableTypes::VEC4)
						("vec4i",      VariableTypes::VEC4I)
						("mat2",       VariableTypes::MAT2)
						("mat3",       VariableTypes::MAT3)
						("mat4",       VariableTypes::MAT4)
						("quaternion", VariableTypes::QUATERNION)
						("GameObject", VariableTypes::GAMEOBJECTHANDLE);

				}

			};

			struct ComponentSymbols : qi::symbols<char, ComponentTypes>
			{

				ComponentSymbols(void)
				{

					add
						("transform", ComponentTypes::TRANSFORM)
						("render",    ComponentTypes::RENDER)
						("camera",    ComponentTypes::CAMERA)
						("rigidbody", ComponentTypes::RIGIDBODY)
						("text2d",    ComponentTypes::TEXT2D)
						("light",     ComponentTypes::LIGHT)
						("keyboard",  ComponentTypes::KEYBOARD)
						("mouse",     ComponentTypes::MOUSE);

				}

			};

			/* Variables */

			struct VariableDefinition
			{
				VariableTypes type;
				std::string name;
				boost::optional<InitValue> initializer;
			};

			template <typename Iterator>
			struct VariableParser :
				qi::grammar<Iterator, VariableDefinition(), SkipGrammar<Iterator>>
			{

				VariableParser(void) : base_type(variable)
				{

					variable %=
						datatype >> qi::no_skip[ qi::omit[' '] ] >> identifier >>
						- ( ( qi::omit['='] >> explicitInitializer ) | implicitInitializer );

				}
				
				qi::rule<Iterator, VariableDefinition(), SkipGrammar<Iterator>> variable;

				DataTypeSymbols datatype;
				Identifier<Iterator> identifier;
				ExplicitInitializerParser<Iterator> explicitInitializer;
				ImplicitInitializerParser<Iterator> implicitInitializer;

				ComponentSymbols component;

			};

			/* Components */

			struct ComponentDefinition
			{
				ComponentTypes component;
				boost::optional<InitValue> initializer;
			};

			template <typename Iterator>
			struct ComponentParser :
				qi::grammar<Iterator, ComponentDefinition(), SkipGrammar<Iterator>>
			{

				ComponentParser(void) : base_type(component)
				{

					component %=
						qi::lit("component") >> qi::no_skip[qi::omit[' ']] >> type >>
						- (initializer);

				}

				qi::rule<Iterator, ComponentDefinition(), SkipGrammar<Iterator>> component;

				ComponentSymbols type;
				ImplicitInitializerParser<Iterator> initializer;

			};

			/* Statements */

			typedef boost::variant<ComponentDefinition, VariableDefinition> Statement;

			template <typename Iterator>
			struct StatementsParser :
				qi::grammar<Iterator, Statement(), SkipGrammar<Iterator>>
			{


				StatementsParser(void) : base_type(statement)
				{

					statement %= component | variable;

				}

				qi::rule<Iterator, boost::variant<ComponentDefinition, VariableDefinition>(), SkipGrammar<Iterator>> statement;
				ComponentParser<Iterator> component;
				VariableParser<Iterator> variable;

			};

			/* Entity */

			struct EntityDefinition
			{
				std::string name;
				std::vector<Statement> statements;
			};

			template <typename Iterator>
			struct EntityParser :
				qi::grammar<Iterator, EntityDefinition(), SkipGrammar<Iterator>>
			{

				EntityParser(void) : base_type(entity)
				{

					entity =
						qi::lit("entity") >> qi::no_skip[qi::omit[' ']] >> identifier [ phoenix::bind(&EntityDefinition::name, qi::_val) = qi::_1 ] >>
						qi::omit['{'] >>
						- statement [ phoenix::push_back(phoenix::bind(&EntityDefinition::statements, qi::_val), qi::_1) ] % ';' >>
						qi::omit['}'] >> - qi::omit[';'];

				}

				qi::rule<Iterator, EntityDefinition(), SkipGrammar<Iterator>> entity;
				StatementsParser<Iterator> statement;
				Identifier<Iterator> identifier;

			};

		}

		class GameObject;

		class EntityInstantiator
		{

		public:

			EntityInstantiator(void) : m_initialized(false) { }

			operator bool () const { return m_initialized; }

			void operator() (GameObject * object) const;


		private:

			entity::EntityDefinition m_entity;
			bool m_initialized;

		};

	}

}

//BOOST_FUSION_ADAPT_STRUCT(
//	mye::core::entity::EntityDefinition,
//	(std::string, name)
//	(std::vector<mye::core::entity::Statement>, statements)
//)

BOOST_FUSION_ADAPT_STRUCT(
	mye::core::entity::InitValue,
	(mye::core::entity::Constructible, constructible)
	(std::vector<mye::core::entity::InitValue>, initializers)
	(mye::core::entity::InitValue::ValueType, value)
)

BOOST_FUSION_ADAPT_STRUCT(
	mye::core::entity::VariableDefinition,
	(mye::core::VariableTypes, type)
	(std::string, name)
	(boost::optional<mye::core::entity::InitValue>, initializer)
)

BOOST_FUSION_ADAPT_STRUCT(
	mye::core::entity::ComponentDefinition,
	(mye::core::ComponentTypes, component)
	(boost::optional<mye::core::entity::InitValue>, initializer)
)