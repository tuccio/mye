//BOOST_FUSION_ADAPT_STRUCT(
//	mye::core::entity::DataMember,
//	(mye::core::entity::DataTypes, data)
//	(std::string, name)
//	(std::vector<std::string>, initializer)
//)
//
//BOOST_FUSION_ADAPT_STRUCT(
//	mye::core::entity::ComponentMember,
//	(mye::core::ComponentTypes, component)
//	(std::string, initializer)
//)




//
//namespace mye
//{
//
//	namespace core {
//
//		namespace entity {
//
//					template <typename Iterator>
//					EntityDefinition<Iterator>::EntityDefinition(void) :
//						EntityDefinition::base_type(start)
//					{
//
//						/* Data members */
//
//						identifier %=
//
//							qi::lexeme[(qi::alpha | qi::char_('_')) >>
//							* (qi::alnum | qi::char_('_'))];
//
//						dataMemberInitializer %=
//
//							(qi::omit['('] >>
//							((qi::lexeme[*(~qi::char_(",);"))]) % ',') >>
//							qi::omit[')']) | qi::omit['='];
//
//						dataMember %=
//
//							dataType >>
//							qi::no_skip[' '] >>
//							identifier >>
//							((dataMemberInitializer >> qi::omit[';']) | qi::omit[';']);
//
//						dataMemberVar %= dataMember;
//
//						componentMemberArguments %=
//							((componentMemberConstructor | qi::lexeme[*(~qi::char_(",)"))]) % qi::char_(',')[qi::_val += ',']) | qi::eps;
//
//						componentMemberConstructor %=
//							qi::hold[(identifier >> *(qi::char_(".:") >> identifier) >> qi::char_('(') >> componentMemberArguments >> qi::char_(')'))];
//
//						componentMember %=
//
//							qi::lit("component") >> componentType >>
//							qi::omit['('] >> componentMemberArguments >> qi::omit[')'] >> qi::omit[';'];
//
//						componentMemberVar %= componentMember;
//
//						start %=
//
//							qi::lit("entity") >>
//
//							qi::no_skip[' '] >>
//							identifier >>
//
//							qi::omit['{'] >>
//
//							//*(dataMemberVar | componentMember) >>
//
//							*(dataMemberVar) >>
//
//							qi::omit['}'];
//
//					}
//
//				}
//		}
//}