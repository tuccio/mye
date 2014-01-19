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

			struct MeshRef
			{
				ResourceHandle handle;
				Mesh *mesh;
				bool resource;
			};

			typedef std::vector<MeshRef> MeshList;

			Model(ResourceManager *owner,
				const std::string &name,
				ManualResourceLoader *manual);

			~Model(void);

			Mesh* AddMesh(void);
			Mesh* AddMesh(const std::string &resourceName);

			const MeshList& GetMeshList(void);

			void Free(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			 MeshList m_meshes;

		};

	}

}
