#pragma once

#include "ResourceManager.h"
#include "Singleton.h"

namespace mye
{

	namespace core
	{

		class ModelManager :
			public ResourceManager,
			public Singleton<ModelManager>
		{

		public:

			ModelManager(void);
			~ModelManager(void);

			mye::core::ResourceHandle CreateImpl(const std::string &name,
				mye::core::ManualResourceLoader *manual,
				mye::core::Resource::ParametersList *params);

			void FreeImpl(mye::core::Resource* resource);

		};

	}

}
