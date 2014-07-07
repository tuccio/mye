#pragma once

#include "Singleton.h"

#include "Allocator.h"
#include "AlignedAllocator.h"

#include <boost/preprocessor/punctuation/comma.hpp>

namespace mye
{

	namespace core
	{

		template <typename T, typename Allocator = DefaultAllocator>
		class PoolAllocator :
			public mye::core::Singleton<PoolAllocator<T, Allocator>>
		{

		public:

			PoolAllocator(void);

			static inline void* Allocate(size_t size);
			static inline void* AllocateNoThrow(size_t size);

			static inline void Free(void *data);

			static void Clear(void);

		private:

			static void *m_free;

		};

	}

}

#include "PoolAllocator.inl"