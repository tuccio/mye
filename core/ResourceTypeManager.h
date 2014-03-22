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

			template <typename ResourceType>
			inline boost::shared_ptr<ResourceType> CreateResource(const String &type,
				const String &name,
				ManualResourceLoader *manual = nullptr,
				const Resource::ParametersList &params = Resource::ParametersList());

			inline boost::shared_ptr<Resource> CreateResource(const String &type,
				const String &name,
				ManualResourceLoader *manual = nullptr,
				const Resource::ParametersList &params = Resource::ParametersList());
			
			template <typename ResourceType>
			inline boost::shared_ptr<ResourceType> GetResource(const String &type,
				const String &name);

			inline boost::shared_ptr<Resource> GetResource(const String &type,
				const String &name);

		private:

			typedef std::unordered_map<String, ResourceManager*> TypeManagerMap;

			TypeManagerMap m_managers;

		};

	}

}

#include "ResourceTypeManager.inl"