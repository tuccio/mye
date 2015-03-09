#pragma once

#include <memory>

namespace mye
{

	namespace core
	{

		struct DefaultAllocator
		{

			static inline void * Allocate(size_t size)
			{

				void *p = malloc(size);

				if (!p)
				{
					throw std::bad_alloc();
				}

				return p;

			}

			static inline void* AllocateNoThrow(size_t size)
			{
				return malloc(size);
			}

			static inline void Free(void *p)
			{
				free(p);
			}

		};

	}

}

#define __MYE_USE_ALLOCATOR(__ALLOCATOR) \
	inline void* operator new(size_t size){ return __ALLOCATOR::Allocate(size); }\
	inline void* operator new(size_t size, const std::nothrow_t &ntv){ return __ALLOCATOR::AllocateNoThrow(size); }\
	inline void* operator new(size_t size, void *p){ return p; }\
	inline void operator delete(void *p) { __ALLOCATOR::Free(p); }\
	inline void operator delete(void *p, void* p2) { }