#pragma once

#include "Singleton.h"

#include "ResourceGroup.h"

#include <string>
#include <map>
#include <unordered_map>

namespace mye
{

	namespace core
	{

		class ResourceManager;

		class ResourceGroupManager :
			public Singleton<ResourceGroupManager>
		{

		public:

			ResourceGroupManager(void);
			~ResourceGroupManager(void);

			/* Resource Managers */

			void RegisterResourceManager(const std::string &resourceType,
				ResourceManager *resourceManager);

			void UnregisterResourceManager(const std::string &resourceType);

			/* Resource Groups */


		private:

			typedef std::unordered_map<std::string, ResourceManager*> ResourceTypesMap;

			ResourceTypesMap _managers;

		};

	}

}
