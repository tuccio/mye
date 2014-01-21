#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

#include "AssimpMeshLoader.h"

#include <cassert>

#undef min
#undef max

using namespace mye::core;

Mesh::Mesh(ResourceManager *resourceManager,
		   const std::string &name,
		   ManualResourceLoader *loader) :
Resource(resourceManager, name, loader),
	m_triangles(0)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::Allocate(VertexDeclaration declaration,
			  size_t triangles)
{
	m_data.Allocate(declaration, 3 * triangles);
	m_triangles = triangles;
}

void Mesh::Allocate(size_t triangles)
{
	m_data.Allocate(3 * triangles);
	m_triangles = triangles;
}

void Mesh::Destroy(void)
{
	m_data.Free();
	m_triangles = 0;
}

size_t Mesh::GetTrianglesCount(void) const
{
	return m_triangles;
}

const VertexDeclaration& Mesh::GetVertexDeclaration(void) const
{
	return m_data.GetVertexDeclaration();
}

const void* Mesh::GetData(void) const
{
	return m_data.GetData();
}

void Mesh::SetVertexAttribute(size_t triangleIndex,
							  size_t vertexIndex,
							  VertexDeclaration::AttributeSemantic semantic,
							  VertexDeclaration::AttributeType type,
							  const void *data)
{
	assert(vertexIndex >= 0 && vertexIndex <= 2);
	m_data.SetVertexAttribute(3 * triangleIndex + vertexIndex, semantic, type, data);
}

void Mesh::GetVertexAttribute(size_t triangleIndex,
							  size_t vertexIndex,
							  VertexDeclaration::AttributeSemantic semantic,
							  VertexDeclaration::AttributeType type,
							  void *data) const
{
	assert(vertexIndex >= 0 && vertexIndex <= 2);
	return m_data.GetVertexAttribute(3 * triangleIndex + vertexIndex, semantic, type, data);
}

bool Mesh::LoadImpl(void)
{
	
	Assimp::Importer importer;
	bool loaded = false;

	const aiScene *scene = importer.ReadFile(m_name,
		aiProcessPreset_TargetRealtime_Quality);
	
	if (scene)
	{

		if (scene->HasMeshes())
		{
			aiMesh *mesh = scene->mMeshes[0];
			AssimpMeshLoader meshLoader(mesh);
			loaded = meshLoader.Load(this);
		}

		importer.FreeScene();

	}
	
	return loaded;

}

void Mesh::UnloadImpl(void)
{
	m_data.Free();
}

size_t Mesh::CalculateSizeImpl(void)
{
	return m_data.GetSize();
}

Mesh::VectorPair Mesh::GetMinMaxVertices(void) const
{
	
	mye::math::Vector3f max = mye::math::Vector3f(
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min()
		);

	mye::math::Vector3f min = mye::math::Vector3f(
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max()
		);

	for (int i = 0; i < m_data.GetVerticesCount(); i++)
	{

		mye::math::Vector3f x;

		m_data.GetVertexAttribute(
			i,
			VertexDeclaration::VDA_POSITION,
			VertexDeclaration::VDAT_FLOAT3,
			&x
			);

		min = min.CwiseMin(x);
		max = max.CwiseMax(x);

	}

	return VectorPair(min, max);

}