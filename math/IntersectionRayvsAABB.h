#pragma once

#include "IntersectionCalculator.h"

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <typename T>
			struct IntersectionCalculator<RayTempl<T>, AABBTempl<T>> :
				IntersectionCalculatorBase<RayTempl<T>, AABBTempl<T>>
			{

				__MYE_MATH_INLINE static bool Intersect(const RayTempl<T> & ray,
				                                        const AABBTempl<T> & aabb,
				                                        T & intersectionDistance)
				{

					mye::math::Matrix<T, 3, 1> invDirection = 1.0f / ray.GetDirection();

					auto aabbMin = aabb.GetMinimum();
					auto aabbMax = aabb.GetMaximum();

					mye::math::Matrix<T, 3, 1> lb(aabbMax.x(), aabbMin.y(), aabbMin.z());
					mye::math::Matrix<T, 3, 1> rt(aabbMin.x(), aabbMax.y(), aabbMax.z());

					T t1 = (lb.x() - ray.GetOrigin().x()) * invDirection.x();
					T t2 = (rt.x() - ray.GetOrigin().x()) * invDirection.x();
					T t3 = (lb.y() - ray.GetOrigin().y()) * invDirection.y();
					T t4 = (rt.y() - ray.GetOrigin().y()) * invDirection.y();
					T t5 = (lb.z() - ray.GetOrigin().z()) * invDirection.z();
					T t6 = (rt.z() - ray.GetOrigin().z()) * invDirection.z();

					float tmin = Max(Max(Min(t1, t2), Min(t3, t4)), Min(t5, t6));
					float tmax = Min(Min(Max(t1, t2), Max(t3, t4)), Max(t5, t6));

					if (tmax < 0)
					{
						intersectionDistance = tmax;
						return false;
					}

					if (tmin > tmax)
					{
						intersectionDistance = tmax;
						return false;
					}

					intersectionDistance = tmin;
					return true;

				}

				__MYE_MATH_INLINE static bool Intersect(const RayTempl<T> & ray,
				                                        const AABBTempl<T> & sphere)
				{
					T t;
					return Intersect(ray, sphere, t);
				}

			};

		}

	}

}