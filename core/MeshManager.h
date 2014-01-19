#pragma once

#include "ResourceManager.h"
#include "Singleton.h"

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

			mye::core::ResourceHandle CreateImpl(const std::string &name,
				mye::core::ManualResourceLoader *manual,
				mye::core::Resource::ParametersList *params);

			void FreeImpl(mye::core::Resource* resource);

		};

	}

}
