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
void GenerateMeshFile(const std::string & filename, aiMesh * mesh);

void CreateScenePropertyTree(const aiScene * scene, const std::string & sceneName, boost::property_tree::ptree & pt, const aiNode * node, const aiMatrix4x4 & world);
boost::filesystem::path MakeMeshName(const std::string & sceneName, int i);

int main(int argc, char * argv[])
{

	if (argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " <obj input file path> <mys output file path>" << std::endl;
		return 1;
	}

	Assimp::Importer importer;	

	const aiScene * scene = importer.ReadFile(argv[1], aiProcessPreset_TargetRealtime_MaxQuality);

	boost::filesystem::path outputFile(argv[2]);
	boost::filesystem::path parent = outputFile.parent_path();

	boost::filesystem::path modelsDirectory = parent / "models";

	boost::filesystem::create_directory(modelsDirectory);

	std::string sceneName = outputFile.stem().string();

	for (int i = 0; i < scene->mNumMeshes; i++)
	{

		boost::filesystem::path meshFile = parent / MakeMeshName(sceneName, i);

		aiMesh * mesh = scene->mMeshes[i];

		GenerateMeshFile(meshFile.string(), mesh);

	}

	boost::property_tree::ptree pt;

	CreateScenePropertyTree(scene, sceneName, pt, scene->mRootNode, aiMatrix4x4());

	boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
	boost::property_tree::write_xml(argv[2], pt, std::locale(), settings);

	return 0;

}

void GenerateMeshFile(const std::string & filename, aiMesh * mesh)
{

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

	exporter.Export(singleMeshScene, "obj", filename);

	delete singleMeshScene->mRootNode;
	delete singleMeshScene->mMeshes;
	delete singleMeshScene->mMaterials;

	free(singleMeshScene);

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

		gameObject.add("render.mesh.<xmlattr>.name", MakeMeshName(sceneName, node->mMeshes[0]).string());
		
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

void FindAABB(const aiMesh * mesh, aiVector3D & min, aiVector3D & max)
{

	if (mesh->mNumVertices)
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

	}

}