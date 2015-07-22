namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> AABBTempl<T>::FromMinMax(const mye::math::Matrix<T, 3, 1> & min,
		                                                        const mye::math::Matrix<T, 3, 1> & max)
		{

			AABBTempl aabb;

			aabb.m_min = min;
			aabb.m_max = max;

			return aabb;

		}

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> AABBTempl<T>::FromCenterHalfExtents(const mye::math::Matrix<T, 3, 1> & center,
		                                                                   const mye::math::Matrix<T, 3, 1> & halfExtents)
		{

			mye::math::Matrix<T, 3, 1> absHalfExtents = halfExtents.CwiseAbs();

			AABBTempl aabb;

			aabb.m_min = center - absHalfExtents;
			aabb.m_max = center + absHalfExtents;

			return aabb;

		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 3, 1> AABBTempl<T>::GetCenter(void) const
		{
			return (m_max + m_min) * T(0.5);
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 3, 1> AABBTempl<T>::GetHalfExtents(void) const
		{
			return (m_max - m_min) * T(0.5);
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 3, 1> AABBTempl<T>::GetMinimum(void) const
		{
			return m_min;
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 3, 1> AABBTempl<T>::GetMaximum(void) const
		{
			return m_max;
		}

		template <typename T>
		__MYE_MATH_INLINE std::array<Matrix<T, 3, 1>, 8> AABBTempl<T>::GetCorners(void) const
		{

			std::array<Matrix<T, 3, 1>, 8> corners;

			corners[static_cast<int>(AABBCorners::LEFT_BOTTOM_NEAR)] = m_min;

			corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_NEAR)].x() = m_max.x();
			corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_NEAR)].y() = m_min.y();
			corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_NEAR)].z() = m_min.z();

			corners[static_cast<int>(AABBCorners::RIGHT_TOP_NEAR)].x() = m_max.x();
			corners[static_cast<int>(AABBCorners::RIGHT_TOP_NEAR)].y() = m_max.y();
			corners[static_cast<int>(AABBCorners::RIGHT_TOP_NEAR)].z() = m_min.z();

			corners[static_cast<int>(AABBCorners::LEFT_TOP_NEAR)].x() = m_min.x();
			corners[static_cast<int>(AABBCorners::LEFT_TOP_NEAR)].y() = m_max.y();
			corners[static_cast<int>(AABBCorners::LEFT_TOP_NEAR)].z() = m_min.z();

			corners[static_cast<int>(AABBCorners::LEFT_BOTTOM_FAR)].x() = m_min.x();
			corners[static_cast<int>(AABBCorners::LEFT_BOTTOM_FAR)].y() = m_min.y();
			corners[static_cast<int>(AABBCorners::LEFT_BOTTOM_FAR)].z() = m_max.z();

			corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_FAR)].x() = m_max.x();
			corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_FAR)].y() = m_min.y();
			corners[static_cast<int>(AABBCorners::RIGHT_BOTTOM_FAR)].z() = m_max.z();

			corners[static_cast<int>(AABBCorners::RIGHT_TOP_FAR)] = m_max;

			corners[static_cast<int>(AABBCorners::LEFT_TOP_FAR)].x() = m_min.x();
			corners[static_cast<int>(AABBCorners::LEFT_TOP_FAR)].y() = m_max.y();
			corners[static_cast<int>(AABBCorners::LEFT_TOP_FAR)].z() = m_max.z();

			return corners;

		}

		template <typename T>
		__MYE_MATH_INLINE Vector3i AABBTempl<T>::GetAxesOrderBySize(void) const
		{

			Matrix<T, 3, 1> halfExtents = GetHalfExtents();

			if (halfExtents.x() < halfExtents.y())
			{

				if (halfExtents.y() < halfExtents.z())
				{
					return Vector3i(0, 1, 2);
				}
				else if (halfExtents.x() < halfExtents.z())
				{
					return Vector3i(0, 2, 1);
				}
				else
				{
					return Vector3i(2, 0, 1);
				}

			}
			else
			{

				if (halfExtents.x() < halfExtents.z())
				{
					return Vector3i(1, 0, 2);
				}
				else if (halfExtents.y() < halfExtents.z())
				{
					return Vector3i(1, 2, 0);
				}
				else
				{
					return Vector3i(2, 1, 0);
				}

			}

		}

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> AABBTempl<T>::TransformAffine(const Matrix<T, 4, 4> & t) const
		{

			auto corners = GetCorners();

			Matrix<T, 4, 1> aabbMin(Infinity());
			Matrix<T, 4, 1> aabbMax(-Infinity());

			for (auto & corner : corners)
			{

				Matrix<T, 4, 1> transformedCorner = t * Matrix<T, 4, 1>(corner, 1);

				aabbMin = aabbMin.CwiseMin(transformedCorner);
				aabbMax = aabbMax.CwiseMax(transformedCorner);

			}

			return AABB::FromMinMax(aabbMin.xyz(), aabbMax.xyz());

		}

		template <typename T>
		__MYE_MATH_INLINE bool AABBTempl<T>::Contains(const Matrix<T, 3, 1> & x) const
		{

			return x.x() <= m_max.x() &&
			       x.y() <= m_max.y() &&
			       x.z() <= m_max.z() &&
			       x.x() >= m_min.x() &&
			       x.y() >= m_min.y() &&
			       x.z() >= m_min.z();

		}

		template <typename T>
		__MYE_MATH_INLINE bool AABBTempl<T>::ContainsStrict(const Matrix<T, 3, 1> & x) const
		{

			return x.x() < m_max.x() &&
			       x.y() < m_max.y() &&
			       x.z() < m_max.z() &&
			       x.x() > m_min.x() &&
			       x.y() > m_min.y() &&
			       x.z() > m_min.z();

		}

		template <typename T>
		__MYE_MATH_INLINE bool AABBTempl<T>::Contains(const AABBTempl<T> & aabb) const
		{
			return Contains(aabb.m_min) && Contains(aabb.m_max);
		}

		template <typename T>
		__MYE_MATH_INLINE bool AABBTempl<T>::ContainsStrict(const AABBTempl<T> &aabb) const
		{
			return ContainsStrict(aabb.m_min) && ContainsStrict(aabb.m_max);
		}

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> AABBTempl<T>::operator+ (const AABBTempl<T> & aabb) const
		{
			return FromMinMax(m_min.CwiseMin(aabb.m_min), m_max.CwiseMax(aabb.m_max));
		}

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> AABBTempl<T>::operator^ (const AABBTempl<T> & aabb) const
		{
			return FromMinMax(m_min.CwiseMax(aabb.m_min), m_max.CwiseMin(aabb.m_max));
		}

	}

}