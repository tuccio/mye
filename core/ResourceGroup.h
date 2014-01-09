#pragma once

#include "FileInfo.h"
#include "Resource.h"
#include "ManualResourceLoader.h"

#include <unordered_map>
#include <map>

namespace mye
{

	namespace core
	{

		class ResourceGroup :
			public INamedObject
		{

		public:


			ResourceGroup(void);
			~ResourceGroup(void);

			void AddResource(const std::string &name,
				const std::string &type,
				ManualResourceLoader *manual = NULL);

			void RemoveResource(const std::string &name);

			void Clear(void);

		private:

			struct ResourceInfo
			{
				std::string type;
				ManualResourceLoader *manual;
			};

			typedef std::unordered_map<std::string, ResourceInfo> ResourcesInfoMap;

			ResourcesInfoMap m_resources;

		};

	}

}

