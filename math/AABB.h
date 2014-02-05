#pragma once

#include <vector>

namespace mye
{

	namespace math
	{

		class AABB
		{

		public:


			enum Corners
			{
				FRONT_LEFT_BOTTOM = 0,
				FRONT_RIGHT_BOTTOM = 1,
				FRONT_RIGHT_TOP = 2,
				FRONT_LEFT_TOP = 3,
				BACK_LEFT_TOP = 4,
				BACK_RIGHT_TOP = 5,
				BACK_RIGHT_BOTTOM = 6,
				BACK_LEFT_BOTTOM = 7
			};

			AABB::AABB(void) :
				m_min(-0.5f, -0.5f, -0.5f),
				m_max(0.5f, 0.5f, 0.5f)
			{
			}

			AABB::~AABB(void)
			{
			}

			static inline AABB FromMinMax(
				const mye::math::Vector3f &min,
				const mye::math::Vector3f &max);

			static inline AABB FromCenterHalfExtents(
				const mye::math::Vector3f &center,
				const mye::math::Vector3f &halfExtents);

			inline Matrix<float, 3, 1> GetCenter(void) const;
			inline Matrix<float, 3, 1> GetHalfExtents(void) const;

			inline Matrix<float, 3, 1> GetMinimum(void) const;
			inline Matrix<float, 3, 1> GetMaximum(void) const;

			inline std::vector<Matrix<float, 3, 1>> GetCorners(void) const;

			inline AABB TransformAffine(const Matrix<float, 4, 4> &t);

			inline bool Contains(const Matrix<float, 3, 1> &x) const;

		private:

			Matrix<float, 3, 1> m_min;
			Matrix<float, 3, 1> m_max;

		};

	}

}

#include "AABB.inl"