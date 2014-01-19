#include "ResourceManager.h"
#include "ResourceTypeManager.h"

using namespace mye::core;

ResourceManager::ResourceManager(const std::string &type) :
	m_type(type)
{
	ResourceTypeManager::GetSingleton().
		RegisterResourceManager(type, this);
}


ResourceManager::~ResourceManager(void)
{
}

ResourceHandle ResourceManager::CreateResource(const std::string &name,
											   ManualResourceLoader *manual,
											   Resource::ParametersList *params)
{

	Lock();

	auto it = m_resources.find(name);

	if (it != m_resources.end())
	{
		return it->second;
	}
	
	ResourceHandle resource(CreateImpl(name, manual, params));

	if (params)
	{
		resource->SetParametersList(*params);
	}

	m_resources[name] = resource;

	Unlock();

	return resource;

}

ResourceHandle ResourceManager::GetResource(const std::string &name)
{

	Lock();

	auto it = m_resources.find(name);

	ResourceHandle handle = (it != m_resources.end()
		           ? it->second :
		           ResourceHandle());

	Unlock();

	return handle;

}

void ResourceManager::FreeResource(const std::string &name)
{

	Lock();

	auto it = m_resources.find(name);

	if (it != m_resources.end())
	{
		_FreeResource(it);
	}

	Unlock();

}

const std::string& ResourceManager::GetType(void) const
{
	return m_type;
}

void ResourceManager::_FreeResource(ResourcesMap::iterator &it)
{
	it->second->Free();
	m_resources.erase(it);
}

ResourceHandle ResourceManager::CreateImpl(const std::string &name,
										   ManualResourceLoader *manual,
										   Resource::ParametersList *params)
{
	return ResourceHandle();
}

void ResourceManager::FreeImpl(Resource *)
{

}