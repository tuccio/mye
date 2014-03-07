#pragma once

#include <map>
#include <memory>

#include "Resource.h"
#include "ResourceHandle.h"

namespace mye
{

	namespace core
	{

		class ResourceManager :
			public Lockable
		{

		public:

			ResourceManager(const String &type);
			~ResourceManager(void);
			
			template <typename ResourceType>
			boost::shared_ptr<ResourceType> CreateResource(const String &name,
				ManualResourceLoader *manual = nullptr,
				Resource::ParametersList *params = nullptr);

			inline boost::shared_ptr<Resource> CreateResource(const String &name,
				ManualResourceLoader *manual = nullptr,
				Resource::ParametersList *params = nullptr);

			template <typename ResourceType>
			boost::shared_ptr<ResourceType> GetResource(const String &name);

			inline boost::shared_ptr<Resource> GetResource(const String &name);

			void FreeResource(const String &name);

			const String& GetType(void) const;

		protected:

			virtual Resource* CreateImpl(const String &name,
				    ManualResourceLoader *manual,
				    Resource::ParametersList *params);

			virtual void FreeImpl(Resource* resource);

			typedef std::map<String, ResourcePointer> ResourcesMap;

			void _FreeResource(ResourcesMap::iterator &it);

			ResourcesMap m_resources;
			String m_type;

		};

	}

}

#include "ResourceManager.inl"