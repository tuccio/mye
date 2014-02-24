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

			void RegisterResourceManager(const String &type,
				ResourceManager *resourceManager);

			void UnregisterResourceManager(const String &type);

			ResourceManager* GetResourceManager(const String &type);

			ResourceHandle CreateResource(const String &type,
				const String &name,
				ManualResourceLoader *manual = nullptr,
				Resource::ParametersList *params = nullptr);

			ResourceHandle GetResource(const String &type,
				const String &name);

		private:

			typedef std::unordered_map<String, ResourceManager*> TypeManagerMap;

			TypeManagerMap m_managers;

		};

	}

}
