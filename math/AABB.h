#pragma once

#include <array>

#include "SSE.h"

namespace mye
{

	namespace math
	{

		enum class AABBCorners
		{
			LEFT_BOTTOM_NEAR  = 0,
			RIGHT_BOTTOM_NEAR = 1,
			RIGHT_TOP_NEAR    = 2,
			LEFT_TOP_NEAR     = 3,
			LEFT_TOP_FAR      = 4,
			RIGHT_TOP_FAR     = 5,
			RIGHT_BOTTOM_FAR  = 6,
			LEFT_BOTTOM_FAR   = 7
		};

		template <typename T>
		class __MYE_MATH_SSE_ALIGNED(16) AABBTempl
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			__MYE_MATH_INLINE AABBTempl(void) :
				m_min(T(-0.5)),
				m_max(T(0.5)) {	}

			__MYE_MATH_INLINE ~AABBTempl(void) { }

			static __MYE_MATH_INLINE AABBTempl FromMinMax(const mye::math::Matrix<T, 3, 1> & min,
			                                              const mye::math::Matrix<T, 3, 1> & max);

			static __MYE_MATH_INLINE AABBTempl FromCenterHalfExtents(const mye::math::Matrix<T, 3, 1> & center,
			                                                         const mye::math::Matrix<T, 3, 1> & halfExtents);

			__MYE_MATH_INLINE Matrix<T, 3, 1> GetCenter(void) const;
			__MYE_MATH_INLINE Matrix<T, 3, 1> GetHalfExtents(void) const;

			__MYE_MATH_INLINE Matrix<T, 3, 1> GetMinimum(void) const;
			__MYE_MATH_INLINE Matrix<T, 3, 1> GetMaximum(void) const;

			__MYE_MATH_INLINE std::array<mye::math::Matrix<T, 3, 1>, 8> GetCorners(void) const;

			__MYE_MATH_INLINE Vector3i GetAxesOrderBySize(void) const;

			__MYE_MATH_INLINE AABBTempl<T> TransformAffine(const Matrix<T, 4, 4> & t) const;

			__MYE_MATH_INLINE bool Contains(const Matrix<T, 3, 1> &x) const;
			__MYE_MATH_INLINE bool Contains(const AABBTempl<T> & aabb) const;

			__MYE_MATH_INLINE bool ContainsStrict(const Matrix<T, 3, 1> & x) const;
			__MYE_MATH_INLINE bool ContainsStrict(const AABBTempl<T> & aabb) const;

			__MYE_MATH_INLINE VolumeSide Intersects(const FrustumTempl<T> & frustum) const;

			__MYE_MATH_INLINE AABBTempl<T> operator+ (const AABBTempl<T> & aabb) const;
			__MYE_MATH_INLINE AABBTempl<T> operator^ (const AABBTempl<T> & aabb) const;

		private:

			Matrix<T, 3, 1> m_min;
			Matrix<T, 3, 1> m_max;

		};

	}

}

#include "AABB.inl"
