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

			ResourceHandle CreateResource(const String &name,
				           ManualResourceLoader *manual = nullptr,
				           Resource::ParametersList *params = nullptr);

			ResourceHandle GetResource(const String &name);

			void FreeResource(const String &name);

			const String& GetType(void) const;

		protected:

			virtual ResourceHandle CreateImpl(const String &name,
				    ManualResourceLoader *manual,
				    Resource::ParametersList *params);

			virtual void FreeImpl(Resource* resource);

			typedef std::map<String, ResourceHandle> ResourcesMap;

			void _FreeResource(ResourcesMap::iterator &it);

			ResourcesMap m_resources;
			String m_type;

		};

	}

}

