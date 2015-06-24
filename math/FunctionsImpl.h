#pragma once

#include <type_traits>

#include "SSE.h"

namespace mye
{
	
	namespace math
	{

		namespace detail
		{

			template <typename T>
			struct Q3InvSqrtMagicNumber;

			template <>
			struct Q3InvSqrtMagicNumber<float> :
				std::integral_constant<unsigned int, 0x5f3759df> { };

			template <>
			struct Q3InvSqrtMagicNumber<double> :
				std::integral_constant<unsigned long long, 0x5fe6ec85e7de30da> { };

			template <typename T>
			struct Q3InvSqrtMagicNumber<const T> : Q3InvSqrtMagicNumber<T> { };


			/*template <typename T, typename Enable = void>
			struct MathFunctions;*/

			template <typename T>
			struct MathFunctions<T, std::enable_if_t<std::is_floating_point<T>::value>>
			{

				__MYE_MATH_INLINE static T Abs(T x)
				{
					return (x < 0 ? -x : x);
				}

				__MYE_MATH_INLINE static T Floor(T x)
				{
					return std::floor(x);
				
				}

				__MYE_MATH_INLINE static T Ceil(T x)
				{
					return std::ceil(x);
				}

				__MYE_MATH_INLINE static T Round(T x)
				{
					return std::round(x);
				}

				__MYE_MATH_INLINE static T Sqrt(T x)
				{	
					return x * InverseSqrt(x);
				}

				__MYE_MATH_INLINE static T InverseSqrt(T x)
				{

					using MagicNumberType = Q3InvSqrtMagicNumber<T>::value_type;

					T halfx = T(0.5) * x;

					MagicNumberType i = * ((MagicNumberType*) &x);

					i = Q3InvSqrtMagicNumber<T>::value - (i >> 1);

					T yi = * ((T*) &i);

					T y0 = yi * (T(1.5) - (halfx * yi * yi));

					return y0;

				}

			};

			template <typename T>
			struct MathFunctions<T, std::enable_if_t<std::is_integral<T>::value>>
			{

				__MYE_MATH_INLINE static T Abs(T x)
				{
					return (x < 0 ? -x : x);
				}

			};

			template <typename T>
			struct MathFunctions<Matrix<T, 2, 1>>
			{

				using VectorType = Matrix<T, 2, 1>;

				__MYE_MATH_INLINE static VectorType Abs(const VectorType & v)
				{
					return v.CwiseAbs();
				}

				__MYE_MATH_INLINE static VectorType Floor(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSEFloor(v.m_vector));
#else
					return VectorType(Floor(v.x()), Floor(v.y()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Ceil(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSECeil(v.m_vector));
#else
					return VectorType(Ceil(v.x()), Ceil(v.y()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Round(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSERound(v.m_vector));
#else
					return VectorType(Round(v.x()), Round(v.y()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Sqrt(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSENewtonRaphson<1>::SquareRootPS(v.m_vector));
#else
					return VectorType(Sqrt(v.x()), Sqrt(v.y()));
#endif
				}

				__MYE_MATH_INLINE static VectorType InverseSqrt(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSENewtonRaphson<1>::ReciprocalSquareRootPS(v.m_vector));
#else
					return VectorType(InverseSqrt(v.x()), InverseSqrt(v.y()));
#endif
				}

			};

			template <typename T>
			struct MathFunctions<Matrix<T, 3, 1>>
			{

				using VectorType = Matrix<T, 3, 1>;

				__MYE_MATH_INLINE static VectorType Abs(const VectorType & v)
				{
					return v.CwiseAbs();
				}

				__MYE_MATH_INLINE static VectorType Floor(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSEFloor(v.m_vector));
#else
					return VectorType(Floor(v.x()), Floor(v.y()), Floor(v.z()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Ceil(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSECeil(v.m_vector));
#else
					return VectorType(Ceil(v.x()), Ceil(v.y()), Ceil(v.z()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Round(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSERound(v.m_vector));
#else
					return VectorType(Round(v.x()), Round(v.y()), Round(v.z()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Sqrt(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSENewtonRaphson<1>::SquareRootPS(v.m_vector));
#else
					return VectorType(Sqrt(v.x()), Sqrt(v.y()), Sqrt(v.z()));
#endif
				}

				__MYE_MATH_INLINE static VectorType InverseSqrt(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSENewtonRaphson<1>::ReciprocalSquareRootPS(v.m_vector));
#else
					return VectorType(InverseSqrt(v.x()), InverseSqrt(v.y()), InverseSqrt(v.z()));
#endif
				}

			};

			template <typename T>
			struct MathFunctions<Matrix<T, 4, 1>>
			{

				using VectorType = Matrix<T, 4, 1>;

				__MYE_MATH_INLINE static VectorType Abs(const VectorType & v)
				{
					return v.CwiseAbs();
				}

				__MYE_MATH_INLINE static VectorType Floor(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSEFloor(v.m_vector));
#else
					return VectorType(Floor(v.x()), Floor(v.y()), Floor(v.z()), Floor(v.w()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Ceil(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSECeil(v.m_vector));
#else
					return VectorType(Ceil(v.x()), Ceil(v.y()), Ceil(v.z()), Ceil(v.w()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Round(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSERound(v.m_vector));
#else
					return VectorType(Round(v.x()), Round(v.y()), Round(v.z()), Round(v.w()));
#endif
				}

				__MYE_MATH_INLINE static VectorType Sqrt(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSENewtonRaphson<1>::SquareRootPS(v.m_vector));
#else
					return VectorType(Sqrt(v.x()), Sqrt(v.y()), Sqrt(v.z()), Sqrt(v.w()));
#endif
				}

				__MYE_MATH_INLINE static VectorType InverseSqrt(const VectorType & v)
				{
#ifdef MYE_USE_SSE
					return VectorType(detail::SSENewtonRaphson<1>::ReciprocalSquareRootPS(v.m_vector));
#else
					return VectorType(InverseSqrt(v.x()), InverseSqrt(v.y()), InverseSqrt(v.z()), InverseSqrt(v.w()));
#endif
				}

			};

		}

	}

}