#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Sphere
		{

		public:

			inline const Matrix<T, 3, 1> GetCenter(void) const;
			inline void SetCenter(const Matrix<T, 3, 1> &center);

			inline T GetRadius(void) const;
			inline void SetRadius(T radius);

		private:

			Matrix<T, 3, 1> m_center;
			T m_radius;

		};

	}

}

#include "Sphere.inl"