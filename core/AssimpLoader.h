#pragma once

#include "Mesh.h"
#include "Model.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>

namespace mye
{

	namespace core
	{

		class AssimpLoader
		{

		public:

			virtual ~AssimpLoader(void) = 0;

			static bool LoadMesh(const String & filename, Mesh * mesh);
			static bool LoadMesh(const aiScene * scene, const aiMesh * assimpMesh, Mesh * mesh);

			static bool LoadModel(const String & filename, Model * model);
			static bool LoadModel(const aiScene * scene, Model * model);

		};

	}

}

