#pragma once

#include <memory>

#include "Resource.h"

namespace mye
{

	namespace core
	{

		class ResourceHandle :
			public boost::shared_ptr<Resource>
		{

		public:

			ResourceHandle(void);
			ResourceHandle(Resource *resource);
			ResourceHandle(Resource &resource);

			~ResourceHandle(void);

			template <typename T>
			T* Cast(void)
			{
				return static_cast<T*>(get());
			}

			template <typename T>
			const T* Cast(void) const
			{
				return static_cast<const T*>(get());
			}
			
			inline void Release(void)
			{
				reset();
			}

		};

	}

}

