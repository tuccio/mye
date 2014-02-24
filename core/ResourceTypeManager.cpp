#include "ResourceTypeManager.h"

using namespace mye::core;

ResourceTypeManager::ResourceTypeManager(void)
{
}


ResourceTypeManager::~ResourceTypeManager(void)
{
}

void ResourceTypeManager::RegisterResourceManager(const String &type,
												   ResourceManager *resourceManager)
{
	m_managers[type] = resourceManager;
}

void ResourceTypeManager::UnregisterResourceManager(const String &type)
{
	m_managers.erase(m_managers.find(type));
}

ResourceManager* ResourceTypeManager::GetResourceManager(const String &type)
{

	auto it = m_managers.find(type);

	if (it != m_managers.end())
	{
		return it->second;
	}

	return nullptr;

}

ResourceHandle ResourceTypeManager::CreateResource(const String &type,
												   const String &name,
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

ResourceHandle ResourceTypeManager::GetResource(const String &type,
												const String &name)
{
	ResourceManager *manager = GetResourceManager(type);

	if (manager)
	{
		return manager->GetResource(name);
	}

	return ResourceHandle();
}