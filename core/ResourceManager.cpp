#include "ResourceManager.h"
#include "ResourceTypeManager.h"

using namespace mye::core;

ResourceManager::ResourceManager(const String &type) :
	m_type(type)
{
	ResourceTypeManager::GetSingleton().
		RegisterResourceManager(type, this);
}


ResourceManager::~ResourceManager(void)
{
}
void ResourceManager::FreeResource(const String &name)
{

	Lock();

	auto it = m_resources.find(name);

	if (it != m_resources.end())
	{
		_FreeResource(it);
	}

	Unlock();

}

const String& ResourceManager::GetType(void) const
{
	return m_type;
}

void ResourceManager::_FreeResource(ResourcesMap::iterator &it)
{
	it->second->Free();
	m_resources.erase(it);
}

Resource* ResourceManager::CreateImpl(const String &name,
									  ManualResourceLoader *manual,
									  const Resource::ParametersList &params)
{
	return nullptr;
}

void ResourceManager::FreeImpl(Resource *)
{

}