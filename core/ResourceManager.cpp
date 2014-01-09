#include "ResourceManager.h"
#include "ResourceGroupManager.h"

using namespace mye::core;

ResourceManager::ResourceManager(const std::string &type) :
	m_type(type)
{
}


ResourceManager::~ResourceManager(void)
{
}

ResourceHandle ResourceManager::CreateResource(const std::string &name,
											   const std::string &group,
											   ManualResourceLoader *manual,
											   Resource::ParametersList *params)
{

	Lock();

	auto it = m_resources.find(name);

	if (it != m_resources.end())
	{
		_FreeResource(it);
	}
	
	ResourceHandle resource(CreateImpl(name, group, manual, params));

	if (params)
	{
		resource->SetParametersList(*params);
	}

	m_resources[name] = resource;

	Unlock();

	/*ResourceGroup *pgroup = ResourceGroupManager::GetSingleton().GetResourceGroup(group);

	// TODO

	if (pgroup)
	{
		
	}
	else
	{

	}*/

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
	//it->second->Unload();
	it->second->Free();
	m_resources.erase(it);
}

ResourceHandle ResourceManager::CreateImpl(const std::string &name,
										   const std::string &group,
										   ManualResourceLoader *manual,
										   Resource::ParametersList *params)
{
	return ResourceHandle();
}

void ResourceManager::FreeImpl(Resource *)
{

}