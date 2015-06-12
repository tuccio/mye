#pragma once

#include <type_traits>

#ifdef MYE_USE_SSE

#include <intrin.h>
#include <xmmintrin.h>
#include <mmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>

#include <cstdlib>
#include <memory>
#include <cstdint>

#include <malloc.h>

#define __MYE_MATH_SSE_ALIGNED(Alignment) __declspec(align(Alignment))

#define __MYE_MATH_SSE_ALIGNED_ALLOCATOR(Alignment) \
	__MYE_MATH_INLINE void * operator new (std::size_t count) { void * p = _aligned_malloc(count, Alignment); if (!p) throw std::bad_alloc(); return p; } \
	__MYE_MATH_INLINE void * operator new[] (std::size_t count) { void * p = _aligned_malloc(count, Alignment); if (!p) throw std::bad_alloc(); return p; } \
	__MYE_MATH_INLINE void * operator new (std::size_t size, void * ptr) { return ptr; }\
	__MYE_MATH_INLINE void operator delete (void * p) { _aligned_free(p); }

namespace mye
{

	namespace math
	{

		namespace detail
		{

			struct IsSSEActive
			{
				static std::true_type value;
			};

		}

	}

}

#include "SSE.inl"

#else /* MYE_USE_SSE */

#define __MYE_MATH_SSE_ALIGNED(Alignment)

#define __MYE_MATH_SSE_ALIGNED_ALLOCATOR(Type, Alignment)

namespace mye
{

	namespace math
	{

		namespace detail
		{

			struct IsSSEActive
			{
				static std::false_type value;
			};

		}

	}

}

#endif /* MYE_USE_SSE */

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <typename T>
			struct Is32BitScalar :
				std::integral_constant<bool,
					std::is_same<T, float>::value ||
					std::is_same<T, int>::value ||
					std::is_same<T, unsigned int>::value> { };

			template <typename T>
			struct IsSSEVectorScalarType :
				std::integral_constant<bool,
					IsSSEActive::value &&
					Is32BitScalar<T>::value> { };

			template <typename Scalar, typename Type = void>
			struct EnableIfSSE :
				std::enable_if<detail::IsSSEActive::value && detail::IsSSEVectorScalarType<Scalar>::value, Type> { };

			template <typename Scalar, typename Type = void>
			struct EnableIfNotSSE :
				std::enable_if<!detail::IsSSEActive::value || !detail::IsSSEVectorScalarType<Scalar>::value, Type> { };



		}

	}

}