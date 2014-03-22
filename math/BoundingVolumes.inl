#undef max
#undef min

namespace mye
{

	namespace math
	{

		template <typename T>
		AABBt<T> BoundingAABB(const AABBt<T> &AABBt)
		{
			return AABBt;
		}

		template <typename T>
		AABBt<T> BoundingAABB(const Triangle<T> &triangle)
		{

			return AABBt<T>::FromMinMax(
				triangle.v0().CwiseMin(triangle.v1().CwiseMin(triangle.v2())),
				triangle.v0().CwiseMax(triangle.v1().CwiseMax(triangle.v2())));

		}

		template <typename T>
		AABBt<T> BoundingAABB(const Frustumt<T> &frustum)
		{

			auto corners = frustum.GetCorners();

			mye::math::Matrix<T, 3, 1> max(std::numeric_limits<T>::min());
			mye::math::Matrix<T, 3, 1> min(std::numeric_limits<T>::max());

			for (int i = 0; i < 8; i++)
			{

				max = max.CwiseMax(corners[i]);
				min = min.CwiseMin(corners[i]);

			}

			return AABBt<T>::FromMinMax(min, max);

		}

	}

}