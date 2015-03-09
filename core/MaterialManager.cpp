#include "MaterialManager.h"


using namespace mye::core;

MaterialManager::MaterialManager(void) :
	ResourceManager("Material") { }


Resource * MaterialManager::CreateImpl(const String & name,
									   ManualResourceLoader * manual,
									   const Parameters & params)
{
	return new Material(this, name, manual);
}

void MaterialManager::FreeImpl(Resource * resource)
{
}