namespace mye
{

	namespace math
	{

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

			corners[FAR_LEFT_BOTTOM] = m_min;

			corners[FAR_RIGHT_BOTTOM].x() = m_max.x();
			corners[FAR_RIGHT_BOTTOM].y() = m_min.y();
			corners[FAR_RIGHT_BOTTOM].z() = m_min.z();

			corners[FAR_RIGHT_TOP].x() = m_max.x();
			corners[FAR_RIGHT_TOP].y() = m_max.y();
			corners[FAR_RIGHT_TOP].z() = m_min.z();

			corners[FAR_LEFT_TOP].x() = m_min.x();
			corners[FAR_LEFT_TOP].y() = m_max.y();
			corners[FAR_LEFT_TOP].z() = m_min.z();

			corners[NEAR_LEFT_BOTTOM].x() = m_min.x();
			corners[NEAR_LEFT_BOTTOM].y() = m_min.y();
			corners[NEAR_LEFT_BOTTOM].z() = m_max.z();

			corners[NEAR_RIGHT_BOTTOM].x() = m_max.x();
			corners[NEAR_RIGHT_BOTTOM].y() = m_min.y();
			corners[NEAR_RIGHT_BOTTOM].z() = m_max.z();

			corners[NEAR_RIGHT_TOP] = m_max;

			corners[NEAR_LEFT_TOP].x() = m_min.x();
			corners[NEAR_LEFT_TOP].y() = m_max.y();
			corners[NEAR_LEFT_TOP].z() = m_max.z();

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

			return AABB(transformedCenter - transformedHalfExtents,
				transformedCenter + transformedHalfExtents);

		}

	}

}