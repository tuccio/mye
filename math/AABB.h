#pragma once

namespace mye
{

	namespace math
	{

		class AABB
		{

		public:


			enum Corners
			{
				NEAR_LEFT_BOTTOM = 0,
				NEAR_RIGHT_BOTTOM = 1,
				NEAR_RIGHT_TOP = 2,
				NEAR_LEFT_TOP = 3,
				FAR_LEFT_TOP = 4,
				FAR_RIGHT_TOP = 5,
				FAR_RIGHT_BOTTOM = 6,
				FAR_LEFT_BOTTOM = 7
			};

			AABB::AABB(void) :
				m_min(-0.5f, -0.5f, -0.5f),
				m_max(0.5f, 0.5f, 0.5f)
			{
			}

			AABB::AABB(const Matrix<float, 3, 1> &min,
				const Matrix<float, 3, 1> &max) :
			m_min(min),
				m_max(max)
			{
			}

			AABB::~AABB(void)
			{
			}

			inline Matrix<float, 3, 1> GetCenter(void) const;
			inline Matrix<float, 3, 1> GetHalfExtents(void) const;

			inline Matrix<float, 3, 1> GetMinimum(void) const;
			inline Matrix<float, 3, 1> GetMaximum(void) const;

			inline std::vector<Matrix<float, 3, 1>> GetCorners(void) const;

			inline AABB TransformAffine(const Matrix<float, 4, 4> &t);

		private:

			Matrix<float, 3, 1> m_min;
			Matrix<float, 3, 1> m_max;

		};

	}

}

#include "AABB.inl"