#include "ResourceGroup.h"
#include "ResourceGroupManager.h"

using namespace mye::core;

ResourceGroup::ResourceGroup(void)
{
}


ResourceGroup::~ResourceGroup(void)
{
}

void ResourceGroup::AddResource(const std::string &name,
								const std::string &type,
								ManualResourceLoader *manual)
{
	
	auto it = _resources.find(name);

	if (it != _resources.end())
	{
		_resources.erase(it);
	}

	ResourceInfo info = {
		type,
		manual
	};

	_resources[name] = info;

}

void ResourceGroup::RemoveResource(const std::string &name)
{

	auto it = _resources.find(name);

	if (it != _resources.end())
	{
		_resources.erase(it);
	}

}

void ResourceGroup::Clear(void)
{

	// TODO

}