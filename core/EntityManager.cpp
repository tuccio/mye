#include "EntityManager.h"

using namespace mye::core;

EntityManager::EntityManager(const String &entityDirectory) :
	ResourceManager("Entity"),
	m_defaultDirectory(entityDirectory)
{
}

Entity * EntityManager::CreateImpl(const String &name,
								  ManualResourceLoader *manual,
								  const Parameters &params)
{
	return new Entity(this, name, manual);
}

void EntityManager::FreeImpl(Resource* resource)
{
	static_cast<Entity*>(resource)->Clear();
}