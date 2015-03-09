#include "Entity.h"
#include "GameObject.h"

#include "EntityManager.h"
#include "EntityParser.h"

#include <fstream>

using namespace mye::core;

Entity::Entity(ResourceManager *owner,
				 const String &name,
				 ManualResourceLoader *manual) :
	Resource(owner, name, manual)
{

}

void Entity::Clear(void)
{
	m_entity.name.clear();
	m_entity.statements.clear();
}

bool Entity::LoadImpl(void)
{
	
	bool loaded = false;

	String directory;

	if (m_params.Contains("directory"))
	{
		directory = m_params.GetString("directory");
	}
	else
	{
		directory = static_cast<EntityManager*>(m_owner)->GetDefaultDirectory();
	}

	String file = directory + m_name + ".mye";

	std::ifstream f(file.CString());

	std::string code = std::string(
		std::istreambuf_iterator<char>(f),
		std::istreambuf_iterator<char>());

	auto it  = code.cbegin();
	auto end = code.cend();

	typedef decltype(it) Iterator;

	if (entity::qi::phrase_parse(it, end, entity::EntityParser<Iterator>(), entity::SkipGrammar<Iterator>(), m_entity) &&
		it == end)
	{
		loaded = true;
	}

	/*entity::EntityParser p(file);

	entity::ParsedEntity e;

	if (p.LexicalParse(e) && p.SemanticParse(e, *this))
	{
		loaded = true;
	}*/

	return loaded;

}

void Entity::UnloadImpl(void)
{
	Clear();
}

size_t Entity::CalculateSizeImpl(void)
{
	return 0;
}


bool Entity::Instantiate(GameObject * object)
{
	return entity::EntityInstantiator::Instantiate(object, m_entity);
}