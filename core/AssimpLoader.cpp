#include "AssimpLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

using namespace mye::core;

AssimpLoader::~AssimpLoader(void)
{
}

bool AssimpLoader::LoadMesh(const String & filename, Mesh * mesh)
{

	Assimp::Importer importer;
	bool loaded = false;

	const aiScene * scene = importer.ReadFile(filename.CString(), aiProcessPreset_TargetRealtime_Quality);

	if (scene && scene->HasMeshes())
	{
		loaded = LoadMesh(scene, scene->mMeshes[0], mesh);
	}

	importer.FreeScene();
	return loaded;

}

bool AssimpLoader::LoadMesh(const aiScene * scene, const aiMesh * assimpMesh, Mesh * mesh)
{

	if (!mesh || !assimpMesh)
	{
		return false;
	}

	Parameters params = mesh->GetParametersList();

	bool normals   = assimpMesh->HasNormals() && (!params.Contains("normals") || params.GetBool("normals"));

	bool texcoord0 = assimpMesh->HasTextureCoords(0) &&	(!params.Contains("texcoords") || params.GetBool("texcoords"));

	bool tangent   = texcoord0 && (!params.Contains("tangent") || params.GetBool("tangent"));
	bool bitangent = texcoord0 && (!params.Contains("bitangent") || params.GetBool("bitangent"));

	//int colorChannels = mesh->GetNumColorChannels();

	VertexDeclaration vDecl;

	vDecl.AddAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT3);

	if (texcoord0)
	{
		vDecl.AddAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2);
	}

	if (normals)
	{
		vDecl.AddAttribute(VertexAttributeSemantic::NORMAL, DataFormat::FLOAT3);
	}

	if (tangent)
	{
		vDecl.AddAttribute(VertexAttributeSemantic::TANGENT, DataFormat::FLOAT3);
	}

	if (bitangent)
	{
		vDecl.AddAttribute(VertexAttributeSemantic::BITANGENT, DataFormat::FLOAT3);
	}

	if ((tangent || bitangent) && !assimpMesh->HasTangentsAndBitangents())
	{
		aiApplyPostProcessing(scene, aiProcess_CalcTangentSpace);
	}

	if (assimpMesh->HasFaces())
	{

		mesh->Allocate(vDecl, assimpMesh->mNumFaces);

		for (size_t triangleIndex = 0; triangleIndex < assimpMesh->mNumFaces; triangleIndex++)
		{

			unsigned int * indices = assimpMesh->mFaces[triangleIndex].mIndices;

			for (int j = 0; j < 3; j++)
			{

				mesh->SetVertexAttribute(triangleIndex,
				                         j,
				                         VertexAttributeSemantic::POSITION,
				                         DataFormat::FLOAT3,
				                         &assimpMesh->mVertices[indices[j]]);

				if (normals)
				{

					mesh->SetVertexAttribute(triangleIndex,
					                         j,
					                         VertexAttributeSemantic::NORMAL,
					                         DataFormat::FLOAT3,
					                         &assimpMesh->mNormals[indices[j]]);

				}

				if (texcoord0)
				{

					mesh->SetVertexAttribute(triangleIndex,
					                         j,
					                         VertexAttributeSemantic::TEXCOORD0,
					                         DataFormat::FLOAT2,
					                         &assimpMesh->mTextureCoords[0][indices[j]]);

				}

				if (tangent)
				{

					mesh->SetVertexAttribute(triangleIndex,
					                         j,
					                         VertexAttributeSemantic::TANGENT,
					                         DataFormat::FLOAT3,
					                         &assimpMesh->mTangents[indices[j]]);

				}

				if (bitangent)
				{

					mesh->SetVertexAttribute(triangleIndex,
					                         j,
					                         VertexAttributeSemantic::BITANGENT,
					                         DataFormat::FLOAT3,
					                         &assimpMesh->mBitangents[indices[j]]);

				}

			}

		}



	}

	return true;

}

bool AssimpLoader::LoadModel(const String & filename, Model * model)
{

	Assimp::Importer importer;
	bool loaded = false;

	const aiScene *scene = importer.ReadFile(filename.CString(), aiProcessPreset_TargetRealtime_Quality);

	if (scene)
	{
		loaded = AssimpLoader::LoadModel(scene, model);
		importer.FreeScene();
	}

	return loaded;

}

bool AssimpLoader::LoadModel(const aiScene * scene, Model * model)
{

	if (!model || !scene)
	{
		return false;
	}

	for (int i = 0; i < scene->mNumMeshes; i++)
	{

		Mesh * mesh = model->AddMesh();
		mesh->SetParametersList(model->GetParametersList());

		LoadMesh(scene, scene->mMeshes[i], mesh);
		mesh->CalculateSize();

	}

	return true;

}