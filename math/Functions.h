#pragma once

#include <cmath>
#include <cfloat>
#include <limits>

namespace mye
{

	namespace math
	{

		template <typename T>
		inline T Abs(T x)
		{
			return (x < 0 ? - x : x);
		}

		template <typename T = Real>
		inline T Epsilon(void)
		{
			return std::numeric_limits<T>::epsilon();
		}

		template <>
		inline float Epsilon<float>(void)
		{
			return FLT_EPSILON;
		}

		template <>
		inline double Epsilon<double>(void)
		{
			return DBL_EPSILON;
		}

		template <typename T>
		inline bool EpsilonEqual(T a, T b, T epsilon = Epsilon<T>())
		{
			return Abs(a - b) <= epsilon;
		}

		template <typename T>
		inline bool EpsilonLessEqual(T a, T b, T epsilon = Epsilon<T>())
		{
			return a <= b + epsilon;
		}

		template <typename T = Real>
		inline T Infinity(void)
		{
			return std::numeric_limits<T>::infinity();
		}

		template <typename T>
		inline T Sqrt(T x)
		{
			return ::sqrt(x);
		}

		template <typename T>
		inline T Min(T a, T b)
		{
			return (a < b ? a : b);
		}

		template <typename T>
		inline T Min(T a, T b, T c)
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
		inline T Max(T a, T b)
		{
			return (a > b ? a : b);
		}

		template <typename T>
		inline T Max(T a, T b, T c)
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