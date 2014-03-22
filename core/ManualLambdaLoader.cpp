#include "ManualLambdaLoader.h"

using namespace mye::core;

ManualLambdaLoader::ManualLambdaLoader(
	std::function<bool(Resource*)> load,
	std::function<void(Resource*)> unload) :
	m_load(load),
	m_unload(unload)
{
}


ManualLambdaLoader::~ManualLambdaLoader(void)
{
}

bool ManualLambdaLoader::Load(Resource *resource)
{
	return m_load(resource);
}

void ManualLambdaLoader::Unload(Resource *resource)
{
	m_unload(resource);
}