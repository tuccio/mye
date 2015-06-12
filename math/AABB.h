#pragma once

#include <vector>

#include "SSE.h"

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
		class __MYE_MATH_SSE_ALIGNED(16) AABBTempl :
			public Volume<T>
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			AABBTempl(void) :
				Volume(VolumeType::AABBt),
				m_min(T(-0.5)),
				m_max(T(0.5))
			{
			}

			~AABBTempl(void)
			{
			}

			static inline AABBTempl FromMinMax(
				const mye::math::Matrix<T, 3, 1> &min,
				const mye::math::Matrix<T, 3, 1> &max);

			static inline AABBTempl FromCenterHalfExtents(
				const mye::math::Matrix<T, 3, 1> &center,
				const mye::math::Matrix<T, 3, 1> &halfExtents);

			inline Matrix<T, 3, 1> GetCenter(void) const;
			inline Matrix<T, 3, 1> GetHalfExtents(void) const;

			inline Matrix<T, 3, 1> GetMinimum(void) const;
			inline Matrix<T, 3, 1> GetMaximum(void) const;

			inline std::vector<Matrix<T, 3, 1>> GetCorners(void) const;

			inline Vector3i GetAxesOrderBySize(void) const;

			inline AABBTempl<T> TransformAffine(const Matrix<T, 4, 4> &t) const;

			inline bool Contains(const Matrix<T, 3, 1> &x) const;
			inline bool Contains(const AABBTempl<T> &aabb) const;

			inline bool ContainsStrict(const Matrix<T, 3, 1> &x) const;
			inline bool ContainsStrict(const AABBTempl<T> &aabb) const;

			AABBTempl* Clone(void) const;

			inline VolumeSide Intersects(const FrustumTempl<T> &frustum) const;

			void TransformAffine(Volume &volume,
				const Matrix<T, 4, 4> &transform) const;

		private:

			Matrix<T, 3, 1> m_min;
			Matrix<T, 3, 1> m_max;

		};

	}

}

#include "AABB.inl"
