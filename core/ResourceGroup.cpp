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
	
	auto it = m_resources.find(name);

	if (it != m_resources.end())
	{
		m_resources.erase(it);
	}

	ResourceInfo info = {
		type,
		manual
	};

	m_resources[name] = info;

}

void ResourceGroup::RemoveResource(const std::string &name)
{

	auto it = m_resources.find(name);

	if (it != m_resources.end())
	{
		m_resources.erase(it);
	}

}

void ResourceGroup::Clear(void)
{

	// TODO

}