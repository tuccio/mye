#pragma once

#include <cmath>
#include <cfloat>
#include <limits>

#include "FunctionsImpl.h"
#include "SSE.h"
#include "Utils.h"

namespace mye
{

	namespace math
	{

		template <typename T = Real>
		__MYE_MATH_INLINE T Epsilon(void)
		{
			return std::numeric_limits<T>::epsilon();
		}

		template <>
		__MYE_MATH_INLINE float Epsilon<float>(void)
		{
			return FLT_EPSILON;
		}

		template <>
		__MYE_MATH_INLINE double Epsilon<double>(void)
		{
			return DBL_EPSILON;
		}

		template <typename T>
		__MYE_MATH_INLINE bool EpsilonEqual(T a, T b, T epsilon = Epsilon<T>())
		{
			return Abs(a - b) <= epsilon;
		}

		template <typename T>
		__MYE_MATH_INLINE bool EpsilonLessEqual(T a, T b, T epsilon = Epsilon<T>())
		{
			return a <= b + epsilon;
		}

		template <typename T = Real>
		__MYE_MATH_INLINE T Infinity(void)
		{
			return std::numeric_limits<T>::infinity();
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEStrippedScalar<T> Abs(T && x)
		{
			return detail::MathFunctions<T>::Abs(std::forward<T>(x));
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEStrippedFP<T> Sqrt(T && x)
		{
			return detail::MathFunctions<T>::Sqrt(std::forward<T>(x));
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEStrippedFP<T> InverseSqrt(T && x)
		{
			return detail::MathFunctions<T>::InverseSqrt(std::forward<T>(x));
		}

		template <typename T>
		__MYE_MATH_INLINE T Min(T a, T b)
		{
			return (a < b ? a : b);
		}

		template <typename T>
		__MYE_MATH_INLINE T Min(T a, T b, T c)
		{
			if (a < b)
			{
				if (a < c)
				{
					return a;
				}
				else
				{
					return c;
				}
			}
			else
			{
				if (b < c)
				{
					return b;
				}
				else
				{
					return c;
				}
			}
		}

		template <typename T>
		__MYE_MATH_INLINE T Max(T a, T b)
		{
			return (a > b ? a : b);
		}

		template <typename T>
		__MYE_MATH_INLINE T Max(T a, T b, T c)
		{

			if (a > b)
			{
				if (a > c)
				{
					return a;
				}
				else
				{
					return c;
				}
			}
			else
			{
				if (b > c)
				{
					return b;
				}
				else
				{
					return c;
				}
			}

		}

	}

}