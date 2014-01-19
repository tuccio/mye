#pragma once

#include "Singleton.h"
#include "ResourceManager.h"

#include <string>
#include <unordered_map>

namespace mye
{

	namespace core
	{

		class ResourceTypeManager :
			public mye::core::Singleton<ResourceTypeManager>
		{

		public:

			ResourceTypeManager(void);
			~ResourceTypeManager(void);

			void RegisterResourceManager(const std::string &type,
				ResourceManager *resourceManager);

			void UnregisterResourceManager(const std::string &type);

			ResourceManager* GetResourceManager(const std::string &type);

			ResourceHandle CreateResource(const std::string &type,
				const std::string &name,
				ManualResourceLoader *manual = NULL,
				Resource::ParametersList *params = NULL);

			ResourceHandle GetResource(const std::string &type,
				const std::string &name);

		private:

			typedef std::unordered_map<std::string, ResourceManager*> TypeManagerMap;

			TypeManagerMap m_managers;

		};

	}

}
