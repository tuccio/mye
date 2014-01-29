#pragma once

#include "Singleton.h"

#include <memory>

namespace mye
{

	namespace core
	{

		template <typename T>
		class PoolAllocator :
			public mye::core::Singleton<PoolAllocator<T>>
		{

		public:

			PoolAllocator(void);

			inline void* Allocate(void);
			inline void* AllocateNoThrow(void);

			inline void Free(void *data);

		private:

			void *m_free;

		};

	}

}

#include "PoolAllocator.inl"