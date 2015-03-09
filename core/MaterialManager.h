#pragma once

#include "Material.h"
#include "ResourceManager.h"

namespace mye
{

	namespace core
	{

		class MaterialManager :
			public ResourceManager
		{

		public:

			MaterialManager(void);

		protected:

			Resource * CreateImpl(const String & name,
										 ManualResourceLoader * manual,
										 const Parameters & params);

			void FreeImpl(Resource * resource);


		};

	}

}