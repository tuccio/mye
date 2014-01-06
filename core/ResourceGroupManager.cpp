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
	_managers[resourceType] = resourceManager;
}

void ResourceGroupManager::UnregisterResourceManager(const std::string &resourceType)
{

	auto it = _managers.find(resourceType);

	_managers.erase(resourceType);

}