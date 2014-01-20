#include "ManualLambdaLoader.h"

using namespace mye::core;

ManualLambdaLoader::ManualLambdaLoader(
	std::function<bool(Resource*)> load,
	std::function<bool(Resource*)> prepare,
	std::function<void(Resource*)> unload) :
	m_load(load),
	m_prepare(prepare),
	m_unload(unload)
{
}


ManualLambdaLoader::~ManualLambdaLoader(void)
{
}

bool ManualLambdaLoader::Load(Resource *resource)
{

	if (m_load(resource))
	{
		CalculateSize(resource);
		return true;
	}

	return false;

}

bool ManualLambdaLoader::Prepare(Resource *resource)
{
	return m_prepare(resource);
}

void ManualLambdaLoader::Unload(Resource *resource)
{
	m_unload(resource);
}