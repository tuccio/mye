#pragma once

#include <cmath>
#include <cfloat>
#include <limits>

namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE T Abs(T x)
		{
			return (x < 0 ? - x : x);
		}

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
		__MYE_MATH_INLINE T Sqrt(T x)
		{
			return ::sqrt(x);
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