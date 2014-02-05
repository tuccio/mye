namespace mye
{

	namespace math
	{

		AABB AABB::FromMinMax(
			const mye::math::Vector3f &min,
			const mye::math::Vector3f &max)
		{
			AABB aabb;
			aabb.m_min = min;
			aabb.m_max = max;
			return aabb;
		}

		AABB AABB::FromCenterHalfExtents(
			const mye::math::Vector3f &center,
			const mye::math::Vector3f &halfExtents)
		{
			mye::math::Vector3f absHalfExtents = halfExtents.CwiseAbs();
			AABB aabb;
			aabb.m_min = center - absHalfExtents;
			aabb.m_max = center + absHalfExtents;
			return aabb;
		}

		Matrix<float, 3, 1> AABB::GetCenter(void) const
		{
			return (m_max + m_min) * 0.5f;
		}

		Matrix<float, 3, 1> AABB::GetHalfExtents(void) const
		{
			return (m_max - m_min) * 0.5f;
		}

		Matrix<float, 3, 1> AABB::GetMinimum(void) const
		{
			return m_min;
		}

		Matrix<float, 3, 1> AABB::GetMaximum(void) const
		{
			return m_max;
		}

		std::vector<Matrix<float, 3, 1>> AABB::GetCorners(void) const
		{

			std::vector<Matrix<float, 3, 1>> corners(8);

			corners[FRONT_LEFT_BOTTOM] = m_min;

			corners[FRONT_RIGHT_BOTTOM].x() = m_max.x();
			corners[FRONT_RIGHT_BOTTOM].y() = m_min.y();
			corners[FRONT_RIGHT_BOTTOM].z() = m_min.z();

			corners[FRONT_RIGHT_TOP].x() = m_max.x();
			corners[FRONT_RIGHT_TOP].y() = m_max.y();
			corners[FRONT_RIGHT_TOP].z() = m_min.z();

			corners[FRONT_LEFT_TOP].x() = m_min.x();
			corners[FRONT_LEFT_TOP].y() = m_max.y();
			corners[FRONT_LEFT_TOP].z() = m_min.z();

			corners[BACK_LEFT_BOTTOM].x() = m_min.x();
			corners[BACK_LEFT_BOTTOM].y() = m_min.y();
			corners[BACK_LEFT_BOTTOM].z() = m_max.z();

			corners[BACK_RIGHT_BOTTOM].x() = m_max.x();
			corners[BACK_RIGHT_BOTTOM].y() = m_min.y();
			corners[BACK_RIGHT_BOTTOM].z() = m_max.z();

			corners[BACK_RIGHT_TOP] = m_max;

			corners[BACK_LEFT_TOP].x() = m_min.x();
			corners[BACK_LEFT_TOP].y() = m_max.y();
			corners[BACK_LEFT_TOP].z() = m_max.z();

			return corners;

		}

		AABB AABB::TransformAffine(const Matrix<float, 4, 4> &t)
		{

			Matrix<float, 3, 1> center = GetCenter();
			Matrix<float, 3, 1> halfExtents = GetHalfExtents();

			Matrix<float, 3, 1> transformedCenter = 
				t * center;

			Matrix<float, 3, 1> transformedHalfExtents = 
				(t * halfExtents).CwiseAbs();

			return FromCenterHalfExtents(transformedCenter,
				transformedHalfExtents);

		}

		bool AABB::Contains(const Matrix<float, 3, 1> &x) const
		{

			return x.x() <= m_max.x() &&
				x.z() <= m_max.y() &&
				x.z() <= m_max.z() &&
				x.x() >= m_min.x() &&
				x.z() >= m_min.y() &&
				x.z() >= m_min.z();

		}

	}

}