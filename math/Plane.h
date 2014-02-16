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
		class Plane
		{

		public:

			Plane(void);
			Plane(const Matrix<T, 3, 1> &p,
				const Matrix<T, 3, 1> &n);

			Plane(T a, T b, T c, T d);

			inline bool Contains(const Matrix<T, 3, 1> &x) const;
			inline PlaneSide Side(const Matrix<T, 3, 1> &x) const;

			inline const Matrix<T, 3, 1>& Normal(void) const;
			inline const T& Coefficient(void) const;

		private:

			Matrix<T, 3, 1> m_normal;
			T m_coefficient;

		};

	}

}

#include "Plane.inl"