#include "ManualResourceLoader.h"
#include "Resource.h"

using namespace mye::core;

ManualResourceLoader::ManualResourceLoader(void)
{
}


ManualResourceLoader::~ManualResourceLoader(void)
{
}

bool ManualResourceLoader::Load(Resource *resource)
{
	return true;
}

void ManualResourceLoader::Unload(Resource *resource)
{
}