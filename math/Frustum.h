#pragma once

#include <array>

#include "SSE.h"

namespace mye
{

	namespace math
	{

		enum class FrustumPlane
		{
			NEAR_PLANE,
			FAR_PLANE,
			LEFT_PLANE,
			RIGHT_PLANE,
			TOP_PLANE,
			BOTTOM_PLANE,
			FIRST = NEAR_PLANE,
			LAST = BOTTOM_PLANE
		};

		enum class FrustumCorners
		{
			LEFT_BOTTOM_NEAR  = 0,
			RIGHT_BOTTOM_NEAR = 1,
			RIGHT_TOP_NEAR    = 2,
			LEFT_TOP_NEAR     = 3,
			LEFT_TOP_FAR      = 4,
			RIGHT_TOP_FAR     = 5,
			RIGHT_BOTTOM_FAR  = 6,
			LEFT_BOTTOM_FAR   = 7,
			FIRST             = LEFT_BOTTOM_NEAR,
			LAST              = LEFT_BOTTOM_FAR
		};
		
		template <typename T>
		class __MYE_MATH_SSE_ALIGNED(16) FrustumTempl
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			FrustumTempl(void);

			FrustumTempl(const Matrix<T, 3, 1> & origin,
			             const Matrix<T, 3, 1> & direction,
			             const Matrix<T, 3, 1> & up,
			             const Matrix<T, 3, 1> & right,
			             T nearPlaneDistance,
			             T farPlaneDistance,
			             T fovX,
			             T fovY);

			std::array<Matrix<T, 3, 1>, 8> GetCorners(void) const;

			inline VolumeSide Side(const Matrix<T, 3, 1> & x) const;

			inline const PlaneTempl<T> & GetPlane(FrustumPlane plane) const;

			VolumeSide Intersects(const AABBTempl<T> & aabb) const;

			void Split(T ratio, FrustumTempl<T> & f1, FrustumTempl<T> & f2) const;

// 			inline typename const Planet<T>& Near(void) const;
// 			inline typename const Planet<T>& Far(void) const;
// 			inline typename const Planet<T>& Left(void) const;
// 			inline typename const Planet<T>& Right(void) const;
// 			inline typename const Planet<T>& Top(void) const;
// 			inline typename const Planet<T>& Bottom(void) const;

		private:

			PlaneTempl<T> m_planes[6];
			

		};

	}

}

#include "Frustum.inl"