#pragma once

#include <malloc.h>
#include <cstdio>
#include <cstddef>

#include "Allocator.h"

#ifdef max
#undef max
#endif

namespace mye
{

	namespace core
	{

		template <int Alignment>
		struct AlignedAllocator
		{

			static inline void * Allocate(size_t size)
			{

				void *p = _aligned_malloc(size, Alignment);

				if (!p)
				{
					throw std::bad_alloc();
				}

				return p;

			}

			static inline void * AllocateNoThrow(size_t size)
			{
				return _aligned_malloc(size, Alignment);
			}

			static inline void Free(void *p)
			{
				_aligned_free(p);
			}

		};

		template <typename T, size_t Alignment>
		struct STLAlignedAllocator
		{

			typedef              T   value_type;
			typedef              T * pointer;
			typedef        const T * const_pointer;
			typedef              T & reference;
			typedef        const T & const_reference;
			typedef    std::size_t   size_type;
			typedef std::ptrdiff_t   difference_type;

			template< class U >
			struct rebind { typedef STLAlignedAllocator<U, Alignment> other; };

			inline pointer       address(reference x) const { return &x; }
			inline const_pointer address(const_reference x) const { return &x; }

			inline pointer allocate(size_type n, std::allocator<void>::const_pointer hint = 0) { return static_cast<pointer>(AlignedAllocator<Alignment>::Allocate();) }
			inline void    deallocate(pointer p, size_type n) { AlignedAllocator<Alignment>::Free(p); }

			inline size_type max_size(void) const { return std::numeric_limits<size_type>::max(); }

			template< class U, class... Args >
			inline void construct(U * p, Args && ... args) { new ((void*)p) U(std::forward(args) ...); }

			template< class U >
			inline void destroy(U * p) { p->~U(); }

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

#define __MYE_DECLARE_ALIGNED_16  __MYE_USE_ALIGNED_ALLOCATOR(16)
#define __MYE_DECLARE_ALIGNED_32  __MYE_USE_ALIGNED_ALLOCATOR(32)
#define __MYE_DECLARE_ALIGNED_64  __MYE_USE_ALIGNED_ALLOCATOR(64)
#define __MYE_DECLARE_ALIGNED_128 __MYE_USE_ALIGNED_ALLOCATOR(128)

#define __MYE_STRUCT_ALIGNED(__NAME, __ALIGNMENT) __MYE_ALIGNED(__ALIGNMENT) struct __NAME

#define __MYE_STRUCT_ALIGNED_16(__NAME)  __MYE_STRUCT_ALIGNED(__NAME,  16)
#define __MYE_STRUCT_ALIGNED_32(__NAME)  __MYE_STRUCT_ALIGNED(__NAME,  32)
#define __MYE_STRUCT_ALIGNED_64(__NAME)  __MYE_STRUCT_ALIGNED(__NAME,  64)
#define __MYE_STRUCT_ALIGNED_128(__NAME) __MYE_STRUCT_ALIGNED(__NAME, 128)

#define __MYE_CLASS_ALIGNED(__NAME, __ALIGNMENT) __MYE_ALIGNED(__ALIGNMENT) class __NAME

#define __MYE_CLASS_ALIGNED_16(__NAME)  __MYE_CLASS_ALIGNED(__NAME,  16)
#define __MYE_CLASS_ALIGNED_32(__NAME)  __MYE_CLASS_ALIGNED(__NAME,  32)
#define __MYE_CLASS_ALIGNED_64(__NAME)  __MYE_CLASS_ALIGNED(__NAME,  64)
#define __MYE_CLASS_ALIGNED_128(__NAME) __MYE_CLASS_ALIGNED(__NAME, 128)