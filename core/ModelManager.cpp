#include "ModelManager.h"
#include "Model.h"

using namespace mye::core;

ModelManager::ModelManager(void) :
	ResourceManager("Model")
{
}


ModelManager::~ModelManager(void)
{
}

ResourceHandle ModelManager::CreateImpl(const String &name,
										ManualResourceLoader *manual,
										Resource::ParametersList *params)
{
	return ResourceHandle(new Model(this, name, manual));
}


void ModelManager::FreeImpl(mye::core::Resource* resource)
{
	static_cast<Model*>(resource)->Free();
}