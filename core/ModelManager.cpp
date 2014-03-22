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

Model* ModelManager::CreateImpl(const String &name,
								ManualResourceLoader *manual,
								const Resource::ParametersList &params)
{
	return (new Model(this, name, manual));
}


void ModelManager::FreeImpl(mye::core::Resource* resource)
{
	static_cast<Model*>(resource)->Clear();
}