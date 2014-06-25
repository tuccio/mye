#pragma once

#include "ResourceManager.h"
#include "Entity.h"

namespace mye
{

	namespace core
	{

		class EntityManager :
			public ResourceManager
		{

		public:

			EntityManager(const String &entityDirectory = "");

		protected:

			Entity* CreateImpl(const String &name,
				ManualResourceLoader *manual,
				const Parameters &params);

			virtual void FreeImpl(Resource* resource);

		};

	}

}