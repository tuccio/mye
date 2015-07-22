#pragma once

#include <map>
#include <memory>

#include "Resource.h"

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
			
			template <typename ResourceType = Resource>
			std::shared_ptr<ResourceType> CreateResource(const String & name,
			                                             ManualResourceLoader * manual = nullptr,
			                                             const Parameters & params = Parameters());

			template <typename ResourceType = Resource>
			std::shared_ptr<ResourceType> GetResource(const String & name);

			void FreeResource(const String &name);

			const String & GetType(void) const;

		protected:

			virtual Resource * CreateImpl(const String & name,
			                              ManualResourceLoader * manual,
			                              const Parameters & params);

			virtual void FreeImpl(Resource * resource);

			typedef std::map<String, ResourcePointer> ResourcesMap;

			void _FreeResource(ResourcesMap::iterator & it);

			ResourcesMap m_resources;
			String m_type;

		};

	}

}

#include "ResourceManager.inl"