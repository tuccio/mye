#pragma once

namespace mye
{

	namespace math
	{

		enum class PlaneSide
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
			PlaneTempl(const Matrix<T, 3, 1> & p, const Matrix<T, 3, 1> & n);

			PlaneTempl(T a, T b, T c, T d);

			__MYE_MATH_INLINE bool Contains(const Matrix<T, 3, 1> &x) const;
			__MYE_MATH_INLINE PlaneSide Side(const Matrix<T, 3, 1> &x) const;

			__MYE_MATH_INLINE Matrix<T, 3, 1> Normal(void) const;
			__MYE_MATH_INLINE const T & Coefficient(void) const;

			__MYE_MATH_INLINE PlaneTempl Transform(const Matrix<T, 4, 4> & t) const;

		private:

			Matrix<T, 4, 1> m_plane;

		};

	}

}

#include "Plane.inl"
