#pragma once

#include "ResourceManager.h"
#include "Singleton.h"
#include "Model.h"

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

			Model* CreateImpl(const String &name,
				ManualResourceLoader *manual,
				Resource::ParametersList *params);

			void FreeImpl(mye::core::Resource* resource);

		};

	}

}
