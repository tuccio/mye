#include "Entity.h"
#include "GameObject.h"

#include "EntityParser.h"

#include <fstream>

using namespace mye::core;

Entity::Entity(ResourceManager *owner,
				 const String &name,
				 ManualResourceLoader *manual) :
	Resource(owner, name, manual)
{

}

void Entity::Insert(Component *component)
{

	m_components.push_back(component);

}

void Entity::Clear(void)
{

	for (Component *c : m_components)
	{
		delete c;
	}

	m_components.clear();

}

bool Entity::LoadImpl(void)
{
	
	bool loaded = false;

	String file = "./entities/" + m_name + ".mye";

	EntityParser p(file);

	ParsedEntity e;

	if (p.LexicalParse(e) && p.SemanticParse(e, *this))
	{
		loaded = true;
	}

	return loaded;

}

void Entity::UnloadImpl(void)
{

	Clear();

}

size_t Entity::CalculateSizeImpl(void)
{

	size_t size = 0;

	for (Component *c : m_components)
	{
		size += sizeof(*c);
	}

	return size;

}