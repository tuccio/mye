#include "AssimpModelLoader.h"
#include "AssimpMeshLoader.h"
#include "Model.h"

using namespace mye::core;

AssimpModelLoader::AssimpModelLoader(void) :
	m_scene(nullptr)
{
}

AssimpModelLoader::AssimpModelLoader(const aiScene *scene) :
	m_scene(scene)
{
}


AssimpModelLoader::~AssimpModelLoader(void)
{
}

bool AssimpModelLoader::Load(Resource *resource)
{

	Model *model = static_cast<Model*>(resource);

	if (!model)
	{
		return false;
	}

	for (int i = 0; i < m_scene->mNumMeshes; i++)
	{
		AssimpMeshLoader meshLoader(m_scene->mMeshes[i]);
		Mesh *mesh = model->AddMesh();
		mesh->SetParametersList(model->GetParametersList());
		meshLoader.Load(mesh);
		mesh->CalculateSize();
	}

	return true;

}

void AssimpModelLoader::Unload(Resource *resource)
{

}