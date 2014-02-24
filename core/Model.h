#pragma once

#include "Mesh.h"
#include "Resource.h"
#include "ResourceHandle.h"

#include <vector>

namespace mye
{

	namespace core
	{

		class Model :
			public Resource
		{

		public:

			Model(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual);

			~Model(void);

			Mesh* AddMesh(void);
			Mesh* AddMesh(const String &resourceName);

			Mesh* GetMesh(int i);
			size_t GetMeshesCount(void) const;

			void Free(void);

			Mesh::VectorPair GetMinMaxVertices(void) const;

		protected:

			struct SubMesh
			{
				ResourceHandle handle;
				Mesh *mesh;
				bool resource;
			};

			typedef std::vector<SubMesh> MeshList;

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			 MeshList m_meshes;

		};

	}

}
