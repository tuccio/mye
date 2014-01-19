#pragma once

#include "ManualResourceLoader.h"

#include <assimp/mesh.h>

namespace mye
{

	namespace core
	{

		class AssimpMeshLoader :
			public ManualResourceLoader
		{

		public:

			AssimpMeshLoader(void);
			AssimpMeshLoader(aiMesh *mesh);
			~AssimpMeshLoader(void);

			bool Load(Resource *resource);
			void Unload(Resource *resource);

		private:

			aiMesh *m_mesh;

		};

	}

}
