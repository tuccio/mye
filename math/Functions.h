#pragma once

#include <cmath>

namespace mye
{

	namespace math
	{

		template <typename T>
		inline T Sqrt(T x)
		{
			return ::sqrt(x);
		}

		template <typename T>
		inline T Abs(T x)
		{
			return (x < 0 ? - x : x);
		}

		template <typename T>
		inline T Min(T a, T b)
		{
			return (a < b ? a : b);
		}

		template <typename T>
		inline T Max(T a, T b)
		{
			return (a > b ? a : b);
		}

	}

}