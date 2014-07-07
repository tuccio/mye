#pragma once

#include <malloc.h>
#include <cstdio>

#include "Allocator.h"

namespace mye
{

	namespace core
	{

		template <int Alignment>
		struct AlignedAllocator
		{

			static inline void* Allocate(size_t size)
			{

				void *p = _aligned_malloc(size, Alignment);

				if (!p)
				{
					throw std::bad_alloc();
				}

				return p;

			}

			static inline void* AllocateNoThrow(size_t size)
			{
				return _aligned_malloc(size, Alignment);
			}

			static inline void  Free(void *p)
			{
				_aligned_free(p);
			}

		};

	}

}

//#define __MYE_USE_ALIGNED_HEAP_ALLOCATION(__ALIGNMENT) \
//	inline void* operator new(size_t size) { void *p = _aligned_malloc(size, __ALIGNMENT); if (p) return p; else throw std::bad_alloc(); }\
//	inline void* operator new(size_t size, const std::nothrow_t &ntv) { return _aligned_malloc(size, __ALIGNMENT); }\
//	inline void* operator new(size_t size, void *p) { return p; }\
//	inline void operator delete(void *p) { _aligned_free(p); }\
//	inline void operator delete(void *p, void* p2) { }

#define __MYE_USE_ALIGNED_ALLOCATOR(__ALIGNMENT)\
	__MYE_USE_ALLOCATOR(mye::core::AlignedAllocator<__ALIGNMENT>)

#define __MYE_ALIGNED(__ALIGNMENT) __declspec(align(__ALIGNMENT))

#define __MYE_DECLARE_ALIGNED_16 __MYE_USE_ALIGNED_ALLOCATOR(16)
#define __MYE_DECLARE_ALIGNED_32 __MYE_USE_ALIGNED_ALLOCATOR(32)
#define __MYE_DECLARE_ALIGNED_64 __MYE_USE_ALIGNED_ALLOCATOR(64)
#define __MYE_DECLARE_ALIGNED_128 __MYE_USE_ALIGNED_ALLOCATOR(128)

#define __MYE_STRUCT_ALIGNED(__NAME, __ALIGNMENT) __MYE_ALIGNED(__ALIGNMENT) struct __NAME

#define __MYE_STRUCT_ALIGNED_16(__NAME) __MYE_STRUCT_ALIGNED(__NAME, 16)
#define __MYE_STRUCT_ALIGNED_32(__NAME) __MYE_STRUCT_ALIGNED(__NAME, 32)
#define __MYE_STRUCT_ALIGNED_64(__NAME) __MYE_STRUCT_ALIGNED(__NAME, 64)
#define __MYE_STRUCT_ALIGNED_128(__NAME) __MYE_STRUCT_ALIGNED(__NAME, 128)

#define __MYE_CLASS_ALIGNED(__NAME, __ALIGNMENT) __MYE_ALIGNED(__ALIGNMENT) class __NAME

#define __MYE_CLASS_ALIGNED_16(__NAME) __MYE_CLASS_ALIGNED(__NAME, 16)
#define __MYE_CLASS_ALIGNED_32(__NAME) __MYE_CLASS_ALIGNED(__NAME, 32)
#define __MYE_CLASS_ALIGNED_64(__NAME) __MYE_CLASS_ALIGNED(__NAME, 64)
#define __MYE_CLASS_ALIGNED_128(__NAME) __MYE_CLASS_ALIGNED(__NAME, 128)