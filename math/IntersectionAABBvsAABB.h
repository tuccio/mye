#pragma once

#include "IntersectionCalculator.h"

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <typename T>
			struct IntersectionCalculator<AABBTempl<T>, AABBTempl<T>> :
				IntersectionCalculatorBase<AABBTempl<T>, AABBTempl<T>>
			{

				__MYE_MATH_INLINE static bool Intersect(const AABBTempl<T> & a,
				                                        const AABBTempl<T> & b)
				{

					Matrix<T, 3, 1> ac = a.GetCenter();
					Matrix<T, 3, 1> ah = a.GetHalfExtents();

					Matrix<T, 3, 1> bc = b.GetCenter();
					Matrix<T, 3, 1> bh = b.GetHalfExtents();

					auto t1 = ac - bc;
					auto t2 = ah + bh;

					return t2.x() > t1.x() &&
					       t2.y() > t1.y() &&
					       t2.z() > t1.z();

				}

			};

		}

	}

}