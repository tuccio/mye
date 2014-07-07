BOOST_FUSION_ADAPT_STRUCT(
	mye::core::DataMember,
	(mye::core::DataTypes, data)
	(std::string, name)
	(std::vector<std::string>, initializer)
)

BOOST_FUSION_ADAPT_STRUCT(
	mye::core::ComponentMember,
	(mye::core::ComponentTypes, component)
	(std::string, initializer)
)

BOOST_FUSION_ADAPT_STRUCT(
	mye::core::ParsedEntity,
	(std::string, name)
	(std::vector<mye::core::EntityMembers>, members)
)

namespace mye
{

	namespace core
	{

		template <typename Iterator>
		EntityDefinition<Iterator>::EntityDefinition(void) :
			EntityDefinition::base_type(start)
		{

			/* Data members */

			identifier %=

				qi::lexeme[(qi::alpha | qi::char_('_')) >>
				* (qi::alnum | qi::char_('_'))];

			dataMemberInitializer %= 

				qi::skip['('] >>
				((qi::lexeme[*(~qi::char_(",)"))]) % ',') >>
				qi::skip[')'];

			dataMember %= 
				
				dataType >>
				qi::no_skip[' '] >>
				identifier >>
				((dataMemberInitializer >> qi::skip[';']) | qi::skip[';']);

			dataMemberVar %= dataMember;

			componentMemberArguments %=
				((componentMemberConstructor | qi::lexeme[*(~qi::char_(",)"))]) % qi::char_(',')[qi::_val += ',']) | qi::eps;

			componentMemberConstructor %=
				qi::hold[(identifier >> *(qi::char_(".:") >> identifier) >> qi::char_('(') >> componentMemberArguments >> qi::char_(')'))];

			componentMember %=

				componentType >>
				qi::skip['('] >> componentMemberArguments >> qi::skip[')'] >> qi::skip[';'];

			componentMemberVar %= componentMember;

			start %=

				qi::lit("entity") >>

				qi::no_skip[' '] >>
				identifier >>

				qi::skip['{'] >>

				*(dataMemberVar | componentMember) >>

				qi::skip['}'];

		}

		template <typename Iterator>
		EntitySkipGrammar<Iterator>::EntitySkipGrammar(void) :
			EntitySkipGrammar::base_type(start)
		{

			commentLine %= qi::lit("//")  >> *(qi::char_ - qi::eol) >> qi::eol;
			commentMultiline %= qi::lit("/*") >> *(qi::char_ - "*/") >> qi::lit("*/");
			start %= ascii::space | commentLine | commentMultiline;

		}

	}
}