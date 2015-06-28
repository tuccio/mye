#pragma once

#include "IntersectionCalculator.h"

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <typename T>
			struct IntersectionCalculator<AABBTempl<T>, SphereTempl<T>> :
				IntersectionCalculatorBase<AABBTempl<T>, SphereTempl<T>>
			{

				__MYE_MATH_INLINE static bool Intersect(const AABBTempl<T> & aabb,
				                                        const SphereTempl<T> & sphere)
				{

					T distance = T(0);

					Vector3 aabbMin = aabb.GetMinimum();
					Vector3 aabbMax = aabb.GetMaximum();

					Vector3 sphereCenter = sphere.GetCenter();
					T       sphereRadius = sphere.GetRadius();

					if (sphereCenter.x() > aabbMax.x())
					{
						distance = distance + sphereCenter.x() - aabbMax.x();
					}
					else if (sphereCenter.x() < aabbMin.x())
					{
						distance = distance + sphereCenter.x() - aabbMin.x();
					}

					if (sphereCenter.y() > aabbMax.y())
					{
						distance = distance + sphereCenter.y() - aabbMax.y();
					}
					else if (sphereCenter.y() < aabbMin.y())
					{
						distance = distance + sphereCenter.y() - aabbMin.y();
					}

					if (sphereCenter.z() > aabbMax.z())
					{
						distance = distance + sphereCenter.z() - aabbMax.z();
					}
					else if (sphereCenter.z() < aabbMin.z())
					{
						distance = distance + sphereCenter.z() - aabbMin.z();
					}

					return distance <= sphereRadius * sphereRadius;

				}

			};

		}

	}

}