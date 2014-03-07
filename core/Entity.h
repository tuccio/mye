#pragma once

#include "String.h"
#include "Components.h"
#include "Resource.h"
#include "ResourceManager.h"

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace mye
{

	namespace core
	{

		class Entity :
			public Resource
		{

		public:

			typedef std::vector<Component*>::iterator Iterator;

			Entity(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual,
				const String &entityDirectory = "");

			void Insert(Component *c);
			void Clear(void);

			inline Iterator begin(void)
			{
				return m_components.begin();
			}

			inline Iterator end(void)
			{
				return m_components.end();
			}

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			std::vector<Component*> m_components;
			String m_entityDirectory;

		};

		typedef boost::shared_ptr<Entity> EntityPointer;

		class EntityTemplateManager :
			public ResourceManager
		{

		public:

			EntityTemplateManager(const String &entityDirectory = "");

		protected:

			Entity* CreateImpl(const String &name,
				ManualResourceLoader *manual,
				Resource::ParametersList *params);

			virtual void FreeImpl(Resource* resource);

		private:

			String m_entityDirectory;

		};

		namespace entity
		{

			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;

			enum TypeEnum
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
				RGB,
				RGBA,
				GAMEOBJECTHANDLE
			};

			struct Member
			{
				TypeEnum type;
				std::string name;
				std::vector<std::string> initializer;
			};

			struct Entity
			{
				std::string name;
				std::vector<entity::Member> members;
			};

			struct Type : qi::symbols<char, TypeEnum>
			{

				Type(void)
				{

					add
						("string", TypeEnum::STRING)
						("bool", TypeEnum::BOOL)
						("int", TypeEnum::INT)
						("float", TypeEnum::FLOAT)
						("vec3", TypeEnum::VEC3)
						("vec3i", TypeEnum::VEC3I)
						("vec4", TypeEnum::VEC4)
						("vec4i", TypeEnum::VEC4I)
						("quaternion", TypeEnum::QUATERNION)
						("RGB", TypeEnum::RGB)
						("RGBA", TypeEnum::RGBA)
						("GameObjectHandle", TypeEnum::GAMEOBJECTHANDLE);

				}

			};

			template <typename Iterator>
			struct EntitySkipGrammar : qi::grammar<Iterator>
			{

				EntitySkipGrammar(void) :
					EntitySkipGrammar::base_type(start)
				{

					commentLine %= qi::lit("//")  >> *(qi::char_ - qi::eol) >> qi::eol;
					commentMultiline %= qi::lit("/*") >> *(qi::char_ - "*/") >> qi::lit("*/");
					start %= ascii::space | commentLine | commentMultiline;

				}

				qi::rule<Iterator> start;
				qi::rule<Iterator> commentLine;
				qi::rule<Iterator> commentMultiline;

			};

			template <typename Iterator>
			struct EntityDefinition : qi::grammar<Iterator, entity::Entity(), EntitySkipGrammar<Iterator>>
			{

				EntityDefinition(void) :
					EntityDefinition::base_type(start)
				{

					identifier %=

						qi::lexeme[(qi::alpha | qi::char_('_')) >>
						* (qi::alnum | qi::char_('_'))];

					initializer %= 

							qi::skip['('] >>
							((qi::lexeme[*(~qi::char_(",)"))]) % ',') >>
							qi::skip[')'];

					start %=

						qi::lit("entity") >>

						qi::no_skip[' '] >>
						identifier >>

						qi::skip['{'] >>

							*(

								type >>
								qi::no_skip[' '] >>
								identifier >>

								(initializer >>
								qi::skip[';'] |
								qi::skip[';'])

							) >>

						qi::skip['}'];

				}

				qi::rule<Iterator, entity::Entity(), EntitySkipGrammar<Iterator>> start;
				qi::rule<Iterator, std::string()> identifier;
				qi::rule<Iterator, std::vector<std::string>(), EntitySkipGrammar<Iterator>> initializer;
				Type type;

			};

			class Parser
			{

			public:

				typedef std::string::const_iterator Iterator;

				Parser(const String &file);
				~Parser(void);

				inline bool LexicalParse(Entity &entity) const
				{

					EntityDefinition<Iterator> entityGrammar;

					Iterator it = m_code.cbegin();
					Iterator end = m_code.cend();

					bool r = qi::phrase_parse(it, end, entityGrammar, EntitySkipGrammar<Iterator>(), entity);

					return (r && it == end);

				}

				bool SemanticParse(Entity &entity, mye::core::Entity &templ) const;

			private:

				std::string m_code;

			};

		}

	}

}

BOOST_FUSION_ADAPT_STRUCT(
	mye::core::entity::Member,
	(mye::core::entity::TypeEnum, type)
	(std::string, name)
	(std::vector<std::string>, initializer)
)

BOOST_FUSION_ADAPT_STRUCT(
	mye::core::entity::Entity,
	(std::string, name)
	(std::vector<mye::core::entity::Member>, members)
)