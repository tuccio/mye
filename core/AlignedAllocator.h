#pragma once

#include <malloc.h>
#include <cstdio>

#define __MYE_USE_ALIGNED_STACK_ALLOCATION(__ALIGNMENT) __declspec(align(__ALIGNMENT))

#define __MYE_USE_ALIGNED_HEAP_ALLOCATION(__ALIGNMENT) \
	inline void* operator new(size_t size) { void *p = _aligned_malloc(size, __ALIGNMENT); if (p) return p; else throw std::bad_alloc(); }\
	inline void* operator new(size_t size, const std::nothrow_t &ntv) { return _aligned_malloc(size, __ALIGNMENT); }\
	inline void* operator new(size_t size, void *p) { return p; }\
	inline void operator delete(void *p) { _aligned_free(p); }\
	inline void operator delete(void *p, void* p2) { }

#define __MYE_ALIGN(__ALIGNMENT) __MYE_USE_ALIGNED_STACK_ALLOCATION(__ALIGNMENT)

#define __MYE_ALIGN_16 __MYE_ALIGN(16)

#define __MYE_ALIGN_16_HEAP __MYE_USE_ALIGNED_HEAP_ALLOCATION(16)