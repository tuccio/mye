#include "AssimpMeshLoader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

using namespace mye::core;

AssimpMeshLoader::AssimpMeshLoader(void) :
	m_mesh(nullptr)
{
}

AssimpMeshLoader::AssimpMeshLoader(aiMesh *mesh) :
	m_mesh(mesh)
{
}


AssimpMeshLoader::~AssimpMeshLoader(void)
{
}

bool AssimpMeshLoader::Load(Resource *resource)
{
	
	Mesh *mesh = static_cast<Mesh*>(resource);

	if (!mesh)
	{
		return false;
	}

	Parameters params = mesh->GetParametersList();

	bool normals = m_mesh->HasNormals() &&
		params.Contains("normals") &&
		params.GetBool("normals");

	bool texcoord0 = m_mesh->HasTextureCoords(0) &&
		params.Contains("texcoords") &&
		params.GetBool("texcoords");

	//int colorChannels = mesh->GetNumColorChannels();

	VertexDeclaration vDecl;

	vDecl.AddAttribute(VertexAttributeSemantic::POSITION,
		VertexAttributeType::FLOAT3);

	if (normals)
	{
		vDecl.AddAttribute(VertexAttributeSemantic::NORMAL,
			VertexAttributeType::FLOAT3);
	}

	if (texcoord0)
	{
		vDecl.AddAttribute(VertexAttributeSemantic::TEXCOORD0,
			VertexAttributeType::FLOAT2);
	}

	if (m_mesh->HasFaces())
	{

		mesh->Allocate(vDecl, m_mesh->mNumFaces);

		for (size_t triangleIndex = 0;
			triangleIndex < m_mesh->mNumFaces;
			triangleIndex++)
		{

			unsigned int *indices = m_mesh->mFaces[triangleIndex].mIndices;

			for (int j = 0; j < 3; j++)
			{

				mesh->SetVertexAttribute(
					triangleIndex,
					j,
					VertexAttributeSemantic::POSITION,
					VertexAttributeType::FLOAT3,
					(unsigned char*) &m_mesh->mVertices[indices[j]]);

				if (normals)
				{

					mesh->SetVertexAttribute(
						triangleIndex,
						j,
						VertexAttributeSemantic::NORMAL,
						VertexAttributeType::FLOAT3,
						(unsigned char*) &m_mesh->mNormals[indices[j]]);

				}

				if (texcoord0)
				{

					mesh->SetVertexAttribute(
						triangleIndex,
						j,
						VertexAttributeSemantic::TEXCOORD0,
						VertexAttributeType::FLOAT2,
						(unsigned char*) &m_mesh->mNormals[indices[j]]);

				}

			}

		}

	}

	return true;

}

void AssimpMeshLoader::Unload(Resource *resource)
{

	Mesh *mesh = static_cast<Mesh*>(resource);

	if (mesh)
	{
		mesh->Clear();
	}

}