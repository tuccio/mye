#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <iostream>

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

	boost::filesystem::path outputFile(argv[1]);
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

		 std::stringstream ss;

		 ss << diffuse.r << " " << diffuse.g << " " << diffuse.b << "1";

		 gameObject.add("render.material.<xmlattr>.diffuse_color", ss.str());
		 gameObject.add("render.material.<xmlattr>.specular", 0);
		 gameObject.add("render.material.<xmlattr>.roughness", 0);
		 gameObject.add("render.material.<xmlattr>.metallic", 0);

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