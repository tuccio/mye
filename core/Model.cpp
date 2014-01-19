#include "Model.h"
#include "ResourceManager.h"
#include "ResourceTypeManager.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "AssimpModelLoader.h"

using namespace mye::core;

Model::Model(ResourceManager *owner,
			 const std::string &name,
			 ManualResourceLoader *manual) :
Resource(owner, name, manual)
{
}


Model::~Model(void)
{
}

Mesh* Model::AddMesh(void)
{

	MeshRef meshRef;
	meshRef.mesh = new Mesh(NULL, "", NULL);
	meshRef.resource = false;

	m_meshes.push_back(meshRef);
	return meshRef.mesh;

}

Mesh* Model::AddMesh(const std::string &resourceName)
{

	ResourceHandle mesh = ResourceTypeManager::GetSingleton().
		CreateResource("Mesh", resourceName);

	MeshRef meshRef;
	meshRef.handle = mesh;
	meshRef.mesh = NULL;
	meshRef.resource = true;

	m_meshes.push_back(meshRef);
	return meshRef.mesh;

}

void Model::Free(void)
{

	for (MeshRef& meshRef : m_meshes)
	{
		if (!meshRef.resource)
		{
			meshRef.mesh->Destroy();
			delete meshRef.mesh;
		}
	}

	m_meshes.clear();

}

bool Model::LoadImpl(void)
{

	Assimp::Importer importer;
	bool loaded = false;

	const aiScene *scene = importer.ReadFile(m_name,
		aiProcessPreset_TargetRealtime_Quality);

	if (scene)
	{
		AssimpModelLoader modelLoader(scene);
		importer.FreeScene();
	}

	return loaded;

}

void Model::UnloadImpl(void)
{
	Free();
}

size_t Model::CalculateSizeImpl(void)
{

	size_t size = 0;

	for (auto meshRef : m_meshes)
	{
		size += meshRef.mesh->GetSize();
	}

	return size;

}

const Model::MeshList& Model::GetMeshList(void)
{
	return m_meshes;
}