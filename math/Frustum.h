#pragma once

#include <vector>

namespace mye
{

	namespace math
	{

		enum class FrustumPlanes
		{
			NEAR_PLANE,
			FAR_PLANE,
			LEFT_PLANE,
			RIGHT_PLANE,
			TOP_PLANE,
			BOTTOM_PLANE
		};

		enum class FrustumCorners
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
		class Frustum :
			public Volume
		{

		public:

			Frustum(void);

			Frustum(const Matrix<T, 3, 1> &origin,
				const Matrix<T, 3, 1>& direction,
				const Matrix<T, 3, 1>& up,
				const Matrix<T, 3, 1>& right,
				T nearPlaneDistance,
				T farPlaneDistance,
				T fovX,
				T fovY);

			std::vector<Matrix<T, 3, 1>> GetCorners(void) const;

			inline VolumeSide Side(const Matrix<T, 3, 1> &x) const;

			inline const Plane<T>& GetPlane(FrustumPlanes plane) const;

// 			inline typename const Plane<T>& Near(void) const;
// 			inline typename const Plane<T>& Far(void) const;
// 			inline typename const Plane<T>& Left(void) const;
// 			inline typename const Plane<T>& Right(void) const;
// 			inline typename const Plane<T>& Top(void) const;
// 			inline typename const Plane<T>& Bottom(void) const;

		private:

			Plane<T> m_planes[6];

		};

	}

}

#include "Frustum.inl"