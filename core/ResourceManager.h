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

			ResourceManager(const std::string &type);
			~ResourceManager(void);

			ResourceHandle CreateResource(const std::string &name,
				           ManualResourceLoader *manual = NULL,
				           Resource::ParametersList *params = NULL);

			ResourceHandle GetResource(const std::string &name);

			void FreeResource(const std::string &name);

			const std::string& GetType(void) const;

		protected:

			virtual ResourceHandle CreateImpl(const std::string &name,
				    ManualResourceLoader *manual,
				    Resource::ParametersList *params);

			virtual void FreeImpl(Resource* resource);

			typedef std::map<std::string, ResourceHandle> ResourcesMap;

			void _FreeResource(ResourcesMap::iterator &it);

			ResourcesMap m_resources;
			std::string m_type;

		};

	}

}

