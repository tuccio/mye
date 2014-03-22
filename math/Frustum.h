#pragma once

#include <vector>

namespace mye
{

	namespace math
	{

		enum class FrustumPlanets
		{
			NEAR_Planet,
			FAR_Planet,
			LEFT_Planet,
			RIGHT_Planet,
			TOP_Planet,
			BOTTOM_Planet,
			FIRST = NEAR_Planet,
			LAST = BOTTOM_Planet
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
			LEFT_BOTTOM_FAR = 7,
			FIRST = LEFT_BOTTOM_NEAR,
			LAST = LEFT_BOTTOM_FAR
		};

		template <typename T>
		class Frustumt :
			public Volume<T>
		{

		public:

			Frustumt(void);

			Frustumt(const Matrix<T, 3, 1> &origin,
				const Matrix<T, 3, 1>& direction,
				const Matrix<T, 3, 1>& up,
				const Matrix<T, 3, 1>& right,
				T nearPlanetDistance,
				T farPlanetDistance,
				T fovX,
				T fovY);

			std::vector<Matrix<T, 3, 1>> GetCorners(void) const;

			inline VolumeSide Side(const Matrix<T, 3, 1> &x) const;

			inline const Planet<T>& GetPlanet(FrustumPlanets Planet) const;

			Frustumt* Clone(void) const;

			VolumeSide Intersects(const AABBt<T> &AABBt) const;

			void TransformAffine(Volume &volume,
				const Matrix<T, 4, 4> &transform) const;

// 			inline typename const Planet<T>& Near(void) const;
// 			inline typename const Planet<T>& Far(void) const;
// 			inline typename const Planet<T>& Left(void) const;
// 			inline typename const Planet<T>& Right(void) const;
// 			inline typename const Planet<T>& Top(void) const;
// 			inline typename const Planet<T>& Bottom(void) const;

		private:

			Planet<T> m_Planets[6];

		};

	}

}

#include "Frustum.inl"