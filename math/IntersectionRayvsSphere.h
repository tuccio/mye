#pragma once

#include "IntersectionCalculator.h"

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <typename T>
			struct IntersectionCalculator<RayTempl<T>, SphereTempl<T>> :
				IntersectionCalculatorBase<RayTempl<T>, SphereTempl<T>>
			{

				__MYE_MATH_INLINE static bool Intersect(const RayTempl<T> & ray,
				                                        const SphereTempl<T> & sphere,
				                                        T & intersectionDistance)
				{

					Matrix<T, 3, 1> rayDirection = ray.GetDirection();
					Matrix<T, 3, 1> rayOrigin    = ray.GetOrigin();

					Matrix<T, 3, 1> sphereCenter = sphere.GetCenter();
					T               sphereRadius = sphere.GetRadius();

					Matrix<T, 3, 1> originCenterDiff = rayOrigin - sphereCenter;

					// Assuming the ray direction is a normalized vector, so A = 1
					//T A = rayDirection.Dot(rayDirection);
					T B = 2 * rayDirection.Dot(originCenterDiff);
					T C = originCenterDiff.Dot(originCenterDiff) - sphereRadius * sphereRadius;

					T delta = B * B - 4 * C;

					
					if (delta >= 0)
					{

						T sqrtDelta = Sqrt(delta);

						T t0 = (- B - sqrtDelta) * T(0.5);

						if (t0 >= 0)
						{
							intersectionDistance = t0;
							return true;
						}
						else
						{

							T t1 = (- B + sqrtDelta) * T(0.5);

							if (t1 >= 0)
							{
								intersectionDistance = t1;
								return true;
							}

						}

					}

					return false;

				}

				__MYE_MATH_INLINE static bool Intersect(const RayTempl<T> & ray,
				                                        const SphereTempl<T> & sphere)
				{
					T t;
					return Intersect(ray, sphere, t);
				}

			};

		}

	}

}