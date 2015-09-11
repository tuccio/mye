#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <iostream>
#include <algorithm>

void FindAABB(const aiMesh * mesh, aiVector3D & min, aiVector3D & max);
void GenerateMeshFiles(boost::filesystem::path & parent, const char * sceneName, aiMesh * mesh, const aiScene * scene, int i);

void CreateScenePropertyTree(const aiScene * scene, const std::string & sceneName, boost::property_tree::ptree & pt, const aiNode * node, const aiMatrix4x4 & world);
boost::filesystem::path MakeMeshName(const std::string & sceneName, int i);
boost::filesystem::path MakeTextureName(const std::string & sceneName, int i, const std::string & type, const std::string & ext);
boost::filesystem::path MakeTextureName(const std::string & sceneName, const std::string & name);

int main(int argc, char * argv[])
{

	if (argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " <obj input file path> <mys output file path>" << std::endl;
		return 1;
	}

	Assimp::Importer importer;	

	unsigned int ppsteps = aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates          | // remove degenerated polygons from the import
		aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
		aiProcess_TransformUVCoords        | // preprocess UV transformations (scaling, translation ...)
		aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes		   | // join small meshes, if possible;
		aiProcess_SplitByBoneCount         | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		0;

	importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.f);

	const aiScene * scene = importer.ReadFile(argv[1], ppsteps | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_ConvertToLeftHanded);

	//aiApplyPostProcessing(scene, aiProcess_ValidateDataStructure | aiProcess_FindInvalidData);

	boost::filesystem::path outputFile(argv[2]);
	boost::filesystem::path parent = outputFile.parent_path();

	boost::filesystem::current_path(parent);

	boost::filesystem::path modelsDirectory = parent / "models";

	boost::filesystem::create_directory(modelsDirectory);

	std::string sceneName = outputFile.stem().string();

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh * mesh = scene->mMeshes[i];
		GenerateMeshFiles(parent, sceneName.c_str(), mesh, scene, i);
	}

	boost::property_tree::ptree pt;

	CreateScenePropertyTree(scene, sceneName, pt, scene->mRootNode, aiMatrix4x4());

	boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
	boost::property_tree::write_xml(argv[2], pt, std::locale(), settings);

	return 0;

}

void GenerateMeshFiles(boost::filesystem::path & parent, const char * sceneName, aiMesh * mesh, const aiScene * scene, int i)
{

	/* Mesh */

	boost::filesystem::path meshFile = parent / MakeMeshName(sceneName, i);

	Assimp::Exporter exporter;

	// Working around an assimp bug with vs
	aiScene * singleMeshScene = (aiScene *) malloc(sizeof(aiScene));

	memset(singleMeshScene, 0, sizeof(aiScene));

	singleMeshScene->mMeshes    = new aiMesh * [1];
	singleMeshScene->mMeshes[0] = new aiMesh(*mesh);
	singleMeshScene->mNumMeshes = 1;

	singleMeshScene->mMeshes[0]->mMaterialIndex = 0;

	singleMeshScene->mMaterials    = new aiMaterial * [1];
	singleMeshScene->mMaterials[0] = new aiMaterial();
	singleMeshScene->mNumMaterials = 1;

	singleMeshScene->mRootNode = new aiNode;

	singleMeshScene->mRootNode->mNumMeshes = 1;
	singleMeshScene->mRootNode->mMeshes    = new unsigned int [1];
	singleMeshScene->mRootNode->mMeshes[0] = 0;

	exporter.Export(singleMeshScene, "obj", meshFile.string());

	delete singleMeshScene->mRootNode;
	delete singleMeshScene->mMeshes;
	delete singleMeshScene->mMaterials;

	free(singleMeshScene);

	/* Texture */

	aiMaterial * meshMaterial = scene->mMaterials[mesh->mMaterialIndex];

	aiString diffusePath, heightPath;

	if (meshMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath) == AI_SUCCESS)
	{

		boost::filesystem::path source = diffusePath.C_Str();
		auto destination = MakeTextureName(sceneName, source.string());

		if (!boost::filesystem::is_regular_file(destination))
		{
			boost::filesystem::copy_file(diffusePath.C_Str(), destination);
			
		}

	}

	if (meshMaterial->GetTexture(aiTextureType_HEIGHT, 0, &heightPath) == AI_SUCCESS)
	{

		boost::filesystem::path source = heightPath.C_Str();
		auto destination = MakeTextureName(sceneName, source.string());

		if (!boost::filesystem::is_regular_file(destination))
		{
			boost::filesystem::copy_file(heightPath.C_Str(), destination);
		}

	}

}

void CreateScenePropertyTree(const aiScene * scene, const std::string & sceneName, boost::property_tree::ptree & pt, const aiNode * node, const aiMatrix4x4 & world)
{

	aiMatrix4x4 currentTransform = world * node->mTransformation;

	// Currently supporting only 1 mesh per object

	//for (int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++)

	if (node->mNumMeshes)
	{

		aiMesh * mesh = scene->mMeshes[node->mMeshes[0]];

		auto & gameObject = pt.add("scene.gameobject", "");

		if (node->mName.length)
		{
			gameObject.add("<xmlattr>.name", node->mName.C_Str());
		}

		std::string meshName = MakeMeshName(sceneName, node->mMeshes[0]).string();

		gameObject.add("render.mesh.<xmlattr>.name", meshName);
		
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
		
		aiColor3D diffuse;
		
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		
		{
			std::stringstream ss;
			ss << diffuse.r << " " << diffuse.g << " " << diffuse.b << "1";
			gameObject.add("render.material.<xmlattr>.diffuse_color", ss.str());
		}
		
		gameObject.add("render.material.<xmlattr>.specular", 0);
		gameObject.add("render.material.<xmlattr>.roughness", 0);
		gameObject.add("render.material.<xmlattr>.metallic", 0);
		
		aiVector3D aabbMin, aabbMax;
		FindAABB(mesh, aabbMin, aabbMax);

		{
			std::stringstream ss;
			ss << aabbMin.x << " " << aabbMin.y << " " << aabbMin.z;
			gameObject.add("render.aabb.min", ss.str());
		}
		
		{
			std::stringstream ss;
			ss << aabbMax.x << " " << aabbMax.y << " " << aabbMax.z;
			gameObject.add("render.aabb.max", ss.str());
		}

		/* Textures */

		aiMaterial * meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
		aiString diffusePath, heightPath;

		if (meshMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath) == AI_SUCCESS)
		{

			boost::filesystem::path source = diffusePath.C_Str();
			auto destination = MakeTextureName(sceneName, source.string());

			gameObject.add("render.diffusetexture.<xmlattr>.name", destination.string());

		}

		if (meshMaterial->GetTexture(aiTextureType_HEIGHT, 0, &heightPath) == AI_SUCCESS)
		{

			boost::filesystem::path source = heightPath.C_Str();
			auto destination = MakeTextureName(sceneName, source.string());

			gameObject.add("render.heightmap.<xmlattr>.name", destination.string());

		}
		

	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		CreateScenePropertyTree(scene, sceneName, pt, node->mChildren[i], currentTransform);
	}

}

boost::filesystem::path MakeMeshName(const std::string & sceneName, int i)
{
	return boost::filesystem::path("models") / (sceneName + "_mesh" + std::to_string(i) + ".obj");
}

boost::filesystem::path MakeTextureName(const std::string & sceneName, int i, const std::string & type, const std::string & ext)
{
	return boost::filesystem::path("textures") / (sceneName + "_" + type + std::to_string(i) + ext);
}

boost::filesystem::path MakeTextureName(const std::string & sceneName, const std::string & name)
{

	auto slashPos = name.find_last_of("\\/");

	if (slashPos != std::string::npos)
	{
		return boost::filesystem::path("textures") / (sceneName + "_" + name.substr(slashPos + 1));
	}
	else
	{
		return boost::filesystem::path("textures") / (sceneName + "_" + name);
	}

}

void FindAABB(const aiMesh * mesh, aiVector3D & min, aiVector3D & max)
{

	min = aiVector3D(10e10f, 10e10f, 10e10f);
	max = aiVector3D(-10e10f, -10e10f, -10e10f);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{

		const aiVector3D v = mesh->mVertices[i];

		if (v.x < min.x)
		{
			min.x = v.x;
		}

		if (v.y < min.y)
		{
			min.y = v.y;
		}

		if (v.z < min.z)
		{
			min.z = v.z;
		}

		if (v.x > max.x)
		{
			max.x = v.x;
		}

		if (v.y > max.y)
		{
			max.y = v.y;
		}

		if (v.z > max.z)
		{
			max.z = v.z;
		}

	}

	/*if (mesh->mNumVertices)
	{

		auto minMaxX = std::minmax(mesh->mVertices,
								   mesh->mVertices + mesh->mNumVertices,
								   [] (const aiVector3D * a, const aiVector3D * b) { return a->x < b->x; });

		auto minMaxY = std::minmax(mesh->mVertices,
								   mesh->mVertices + mesh->mNumVertices,
								   [] (const aiVector3D * a, const aiVector3D * b) { return a->y < b->y; });

		auto minMaxZ = std::minmax(mesh->mVertices,
								   mesh->mVertices + mesh->mNumVertices,
								   [] (const aiVector3D * a, const aiVector3D * b) { return a->z < b->z; });

		min.x = minMaxX.first->x;
		min.y = minMaxY.first->y;
		min.z = minMaxZ.first->z;

		max.x = minMaxX.second->x;
		max.y = minMaxY.second->y;
		max.z = minMaxZ.second->z;

	}*/

}