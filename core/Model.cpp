#include "Model.h"
#include "ResourceManager.h"
#include "ResourceTypeManager.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "AssimpLoader.h"

using namespace mye::core;

namespace mye
{
	namespace core
	{
		namespace detail
		{

			struct ModelMeshGrabber :
				public boost::static_visitor<Mesh*>
			{

				Mesh* operator() (MeshPointer m)
				{
					return m.get();
				}

				Mesh* operator() (Mesh *m)
				{
					return m;
				}

			};

			struct ModelConstMeshGrabber :
				public boost::static_visitor<const Mesh*>
			{

				const Mesh* operator() (const MeshPointer m)
				{
					return m.get();
				}

				const Mesh* operator() (const Mesh *m)
				{
					return m;
				}

			};

			struct ModelOwnedMeshesCleaner :
				public boost::static_visitor<void>
			{

				void operator() (MeshPointer m)
				{
				}

				void operator() (Mesh *m)
				{
					delete m;
				}

			};

		}
	}
}


Model::Model(ResourceManager *owner,
			 const String &name,
			 ManualResourceLoader *manual) :
Resource(owner, name, manual)
{
}


Model::~Model(void)
{
}

Mesh* Model::AddMesh(void)
{

	Mesh *m = new Mesh(nullptr, "", nullptr);

	m_meshes.push_back(SubMesh(m));
	return m;

}

Mesh* Model::AddMesh(const String &resourceName)
{

	MeshPointer mesh = ResourceTypeManager::GetSingleton().
		CreateResource<Mesh>("Mesh", resourceName);

	mesh->Load();

	m_meshes.push_back(SubMesh(mesh));

	return mesh.get();

}

Mesh* Model::GetMesh(int i)
{

	detail::ModelMeshGrabber v;
	return (m_meshes[i].apply_visitor(v));

}

size_t Model::GetMeshesCount(void) const
{
	return m_meshes.size();
}

void Model::Clear(void)
{

	detail::ModelOwnedMeshesCleaner v;

	for (SubMesh& meshRef : m_meshes)
	{
		meshRef.apply_visitor(v);
	}

	m_meshes.clear();

}

bool Model::LoadImpl(void)
{

	return AssimpLoader::LoadModel(m_name, this);

}

void Model::UnloadImpl(void)
{
	Clear();
}


size_t Model::CalculateSizeImpl(void)
{

	size_t size = 0;
	detail::ModelMeshGrabber v;

	for (SubMesh &meshRef : m_meshes)
	{

		size += meshRef.apply_visitor(v)->GetSize();

	}

	return size;

}

Mesh::VectorPair Model::GetMinMaxVertices(void) const
{

	mye::math::Vector3 max = mye::math::Vector3(
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min()
		);

	mye::math::Vector3 min = mye::math::Vector3(
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max()
		);

	for (const SubMesh &meshRef : m_meshes)
	{

		Mesh::VectorPair localMinMax;
		detail::ModelConstMeshGrabber v;

		localMinMax = meshRef.apply_visitor(v)->GetMinMaxVertices();

		min = min.CwiseMin(localMinMax.first);
		max = max.CwiseMax(localMinMax.second);

	}

	return Mesh::VectorPair(min, max);

}