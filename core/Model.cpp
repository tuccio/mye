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

	SubMesh meshRef;
	meshRef.mesh = new Mesh(NULL, "", NULL);
	meshRef.resource = false;

	m_meshes.push_back(meshRef);
	return meshRef.mesh;

}

Mesh* Model::AddMesh(const std::string &resourceName)
{

	ResourceHandle mesh = ResourceTypeManager::GetSingleton().
		CreateResource("Mesh", resourceName);

	mesh->Load();

	SubMesh meshRef;
	meshRef.handle = mesh;
	meshRef.mesh = NULL;
	meshRef.resource = true;

	m_meshes.push_back(meshRef);

	return meshRef.handle.Cast<Mesh>();

}

Mesh* Model::GetMesh(int i)
{
	return (m_meshes[i].resource ?
		m_meshes[i].handle.Cast<Mesh>() :
		m_meshes[i].mesh);
}

size_t Model::GetMeshesCount(void) const
{
	return m_meshes.size();
}

void Model::Free(void)
{

	for (SubMesh& meshRef : m_meshes)
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
		loaded = modelLoader.Load(static_cast<Resource*>(this));
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

		if (!meshRef.resource)
		{
			size += meshRef.mesh->GetSize();
		}
		else
		{
			size += meshRef.handle.Cast<Mesh>()->GetSize();
		}

	}

	return size;

}

Mesh::VectorPair Model::GetMinMaxVertices(void) const
{

	Eigen::Vector3f max = Eigen::Vector3f(
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min()
		);

	Eigen::Vector3f min = Eigen::Vector3f(
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max()
		);

	for (auto &meshRef : m_meshes)
	{

		Mesh::VectorPair localMinMax;

		if (!meshRef.resource)
		{
			localMinMax = meshRef.mesh->GetMinMaxVertices();
		}
		else
		{
			localMinMax = meshRef.handle.Cast<Mesh>()->GetMinMaxVertices();
		}

		min = min.cwiseMin(localMinMax.first);
		max = max.cwiseMax(localMinMax.second);

	}

	return Mesh::VectorPair(min, max);

}