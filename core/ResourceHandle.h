#pragma once

#include <memory>

#include "Resource.h"

namespace mye
{

	namespace core
	{

		class ResourceHandle :
			public std::shared_ptr<Resource>
		{

		public:

			ResourceHandle(void);
			ResourceHandle(Resource *resource);
			ResourceHandle(Resource &resource);

			~ResourceHandle(void);

		};

	}

}

