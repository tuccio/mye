#pragma once

#include <vector>

namespace mye
{

	namespace math
	{

		enum class AABBCorners
		{
			LEFT_BOTTOM_NEAR = 0,
			RIGHT_BOTTOM_NEAR = 1,
			RIGHT_TOP_NEAR = 2,
			LEFT_TOP_NEAR = 3,
			LEFT_TOP_FAR = 4,
			RIGHT_TOP_FAR = 5,
			RIGHT_BOTTOM_FAR = 6,
			LEFT_BOTTOM_FAR = 7
		};

		template <typename T>
		class AABB :
			public Volume
		{

		public:

			AABB(void) :
				Volume(VolumeType::AABB),
				m_min(T(-0.5)),
				m_max(T(0.5))
			{
			}

			~AABB(void)
			{
			}

			static inline AABB FromMinMax(
				const mye::math::Matrix<T, 3, 1> &min,
				const mye::math::Matrix<T, 3, 1> &max);

			static inline AABB FromCenterHalfExtents(
				const mye::math::Matrix<T, 3, 1> &center,
				const mye::math::Matrix<T, 3, 1> &halfExtents);

			inline Matrix<T, 3, 1> GetCenter(void) const;
			inline Matrix<T, 3, 1> GetHalfExtents(void) const;

			inline Matrix<T, 3, 1> GetMinimum(void) const;
			inline Matrix<T, 3, 1> GetMaximum(void) const;

			inline std::vector<Matrix<T, 3, 1>> GetCorners(void) const;

			inline Vector3i GetAxesOrderBySize(void) const;

			inline AABB<T> TransformAffine(const Matrix<T, 4, 4> &t) const;

			inline bool Contains(const Matrix<T, 3, 1> &x) const;
			inline bool Contains(const AABB<T> &aabb) const;

			inline bool ContainsStrict(const Matrix<T, 3, 1> &x) const;
			inline bool ContainsStrict(const AABB<T> &aabb) const;

		private:

			Matrix<T, 3, 1> m_min;
			Matrix<T, 3, 1> m_max;

		};

	}

}

#include "AABB.inl"