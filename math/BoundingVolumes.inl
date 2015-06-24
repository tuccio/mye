#undef max
#undef min

namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> BoundingAABB(const AABBTempl<T> & aabb)
		{
			return aabb;
		}

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> BoundingAABB(const Triangle<T> & triangle)
		{

			return AABBTempl<T>::FromMinMax(
				triangle.v0().CwiseMin(triangle.v1().CwiseMin(triangle.v2())),
				triangle.v0().CwiseMax(triangle.v1().CwiseMax(triangle.v2())));

		}

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> BoundingAABB(const FrustumTempl<T> & frustum)
		{

			auto corners = frustum.GetCorners();

			mye::math::Matrix<T, 3, 1> max(std::numeric_limits<T>::min());
			mye::math::Matrix<T, 3, 1> min(std::numeric_limits<T>::max());

			for (int i = 0; i < 8; i++)
			{

				max = max.CwiseMax(corners[i]);
				min = min.CwiseMin(corners[i]);

			}

			return AABBTempl<T>::FromMinMax(min, max);

		}

	}

}