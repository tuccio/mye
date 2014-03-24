#pragma once

namespace mye
{

	namespace math
	{

		enum class PlanetSide
		{
			FRONT,
			INSIDE,
			BACK
		};

		template <typename T>
		class PlaneTempl
		{

		public:

			PlaneTempl(void);
			PlaneTempl(const Matrix<T, 3, 1> &p,
				const Matrix<T, 3, 1> &n);

			PlaneTempl(T a, T b, T c, T d);

			inline bool Contains(const Matrix<T, 3, 1> &x) const;
			inline PlanetSide Side(const Matrix<T, 3, 1> &x) const;

			inline const Matrix<T, 3, 1>& Normal(void) const;
			inline const T& Coefficient(void) const;

			inline PlaneTempl Transformt(const Matrix<T, 4, 4> &transform) const;

		private:

			Matrix<T, 3, 1> m_normal;
			T m_coefficient;

		};

	}

}

#include "Plane.inl"
