#include "ResourceGroupManager.h"

using namespace mye::core;

ResourceGroupManager::ResourceGroupManager(void)
{
}


ResourceGroupManager::~ResourceGroupManager(void)
{
}

void ResourceGroupManager::RegisterResourceManager(const std::string &resourceType,
							 ResourceManager *resourceManager)
{
	m_managers[resourceType] = resourceManager;
}

void ResourceGroupManager::UnregisterResourceManager(const std::string &resourceType)
{

	auto it = m_managers.find(resourceType);

	m_managers.erase(resourceType);

}