#include "Mesh.h"

#include "AssimpLoader.h"

#include <cassert>

#undef min
#undef max

using namespace mye::core;

Mesh::Mesh(ResourceManager *resourceManager,
		   const String &name,
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

void Mesh::Clear(void)
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
							  VertexAttributeSemantic semantic,
							  DataFormat type,
							  const void *data)
{
	assert(vertexIndex >= 0 && vertexIndex <= 2);
	m_data.SetVertexAttribute(3 * triangleIndex + vertexIndex, semantic, type, data);
}

void Mesh::GetVertexAttribute(size_t triangleIndex,
							  size_t vertexIndex,
							  VertexAttributeSemantic semantic,
							  DataFormat type,
							  void *data) const
{
	assert(vertexIndex >= 0 && vertexIndex <= 2);
	return m_data.GetVertexAttribute(3 * triangleIndex + vertexIndex, semantic, type, data);
}

bool Mesh::LoadImpl(void)
{
	
	return AssimpLoader::LoadMesh(m_name, this);

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

	for (int i = 0; i < m_data.GetVerticesCount(); i++)
	{

		mye::math::Vector3 x;

		m_data.GetVertexAttribute(
			i,
			VertexAttributeSemantic::POSITION,
			DataFormat::FLOAT3,
			&x
			);

		min = min.CwiseMin(x);
		max = max.CwiseMax(x);

	}

	return VectorPair(min, max);

}