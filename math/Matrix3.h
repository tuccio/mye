#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 3, 3>
		{

		public:

			Matrix(void);

			inline void Fill(T x);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline Matrix<T, 3, 3> operator* (
				const Matrix<T, 3, 3> &a);

			inline T* Data(void);
			inline const T* Data(void) const;

		private:

			T m_data[9];

		};
		
		typedef Matrix<double, 3, 3> Matrix3d;
		typedef Matrix<float, 3, 3> Matrix3f;
		typedef Matrix<int, 3, 3> Matrix3i;
		typedef Matrix<unsigned int, 3, 3> Matrix3u;

	}

}

#include "Matrix3.inl"