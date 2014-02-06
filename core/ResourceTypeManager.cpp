#include "ResourceTypeManager.h"

using namespace mye::core;

ResourceTypeManager::ResourceTypeManager(void)
{
}


ResourceTypeManager::~ResourceTypeManager(void)
{
}

void ResourceTypeManager::RegisterResourceManager(const std::string &type,
												   ResourceManager *resourceManager)
{
	m_managers[type] = resourceManager;
}

void ResourceTypeManager::UnregisterResourceManager(const std::string &type)
{
	m_managers.erase(m_managers.find(type));
}

ResourceManager* ResourceTypeManager::GetResourceManager(const std::string &type)
{

	auto it = m_managers.find(type);

	if (it != m_managers.end())
	{
		return it->second;
	}

	return nullptr;

}

ResourceHandle ResourceTypeManager::CreateResource(const std::string &type,
												   const std::string &name,
												   ManualResourceLoader *manual,
												   Resource::ParametersList *params)
{

	ResourceManager *manager = GetResourceManager(type);

	if (manager)
	{
		return manager->CreateResource(name, manual, params);
	}

	return ResourceHandle();

}

ResourceHandle ResourceTypeManager::GetResource(const std::string &type,
												const std::string &name)
{
	ResourceManager *manager = GetResourceManager(type);

	if (manager)
	{
		return manager->GetResource(name);
	}

	return ResourceHandle();
}