#pragma once

#include <map>
#include <memory>

#include "Resource.h"
#include "ResourceHandle.h"

#define MYE_DEFAULT_GROUP ""

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
				           const std::string &group = MYE_DEFAULT_GROUP,
				           ManualResourceLoader *manual = NULL,
				           Resource::ParametersList *params = NULL);

			ResourceHandle GetResource(const std::string &name);

			void FreeResource(const std::string &name);

			const std::string& GetType(void) const;

		protected:

			virtual ResourceHandle CreateImpl(const std::string &name,
				    const std::string &group,
				    ManualResourceLoader *manual,
				    Resource::ParametersList *params);

			virtual void FreeImpl(Resource* resource);
			

		private:

			typedef std::map<std::string, ResourceHandle> ResourcesMap;

			void _FreeResource(ResourcesMap::iterator &it);

			ResourcesMap _resources;
			std::string _type;

		};

	}

}

