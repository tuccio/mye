#pragma once

#include "ResourceManager.h"
#include "Singleton.h"
#include "Mesh.h"

namespace mye
{

	namespace core
	{

		class MeshManager :
			public ResourceManager,
			public Singleton<MeshManager>
		{

		public:

			MeshManager(void);
			~MeshManager(void);

			Mesh* CreateImpl(const String &name,
				ManualResourceLoader *manual,
				const Parameters &params);

			void FreeImpl(mye::core::Resource* resource);

		};

	}

}
