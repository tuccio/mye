#include "ManualResourceLoader.h"

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

bool ManualResourceLoader::Prepare(Resource *resource)
{
	return true;
}

void ManualResourceLoader::Unload(Resource *resource)
{
}