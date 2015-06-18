#pragma once

#include "IntersectionCalculator.h"

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <typename T>
			struct IntersectionCalculator<AABBTempl<T>, FrustumTempl<T>> :
				IntersectionCalculatorBase<AABBTempl<T>, FrustumTempl<T>>
			{
				
				__MYE_MATH_INLINE static bool Intersect(const AABBTempl<T> & aabb,
				                                        const FrustumTempl<T> & frustum)
				{
					for (int i = 0; i < 6; i++)
					{

						const PlaneTempl<T> & plane = frustum.GetPlane(static_cast<FrustumPlane>(i));

						Matrix<T, 3, 1> normal = plane.Normal();
						T d = plane.Coefficient();

						Matrix<T, 3, 1> min = aabb.GetMinimum();
						Matrix<T, 3, 1> max = aabb.GetMaximum();

						Matrix<T, 3, 1> n, p;

						if (normal.x() >= 0)
						{
							n.x() = min.x();
							p.x() = max.x();
						}
						else
						{
							n.x() = max.x();
							p.x() = min.x();
						}

						if (normal.y() >= 0)
						{
							n.y() = min.y();
							p.y() = max.y();
						}
						else
						{
							n.y() = max.y();
							p.y() = min.y();
						}

						if (normal.z() >= 0)
						{
							n.z() = min.z();
							p.z() = max.z();
						}
						else
						{
							n.z() = max.z();
							p.z() = min.z();
						}

						if (normal.Dot(n) + d > 0)
						{
							return false; // AABB is outside the frustum
						}

						if (normal.Dot(p) + d >= 0)
						{
							return true; // Intersection
						}

					}

					return false; // AABB is inside the frustum
				}

			};

		}

	}

}