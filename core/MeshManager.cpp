#include "MeshManager.h"
#include "Model.h"

using namespace mye::core;

MeshManager::MeshManager(void) :
	ResourceManager("Mesh")
{
}


MeshManager::~MeshManager(void)
{
}

Mesh* MeshManager::CreateImpl(const String &name,
										ManualResourceLoader *manual,
										Resource::ParametersList *params)
{
	return (new Mesh(this, name, manual));
}


void MeshManager::FreeImpl(mye::core::Resource* resource)
{
	static_cast<Model*>(resource)->Free();
}