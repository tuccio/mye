#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 4, 4>
		{

		public:

			Matrix(void);

			inline void Fill(T x);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline Matrix<T, 4, 4> operator* (
				const Matrix<T, 4, 4> &a);

			inline T* Data(void);
			inline const T* Data(void) const;

		private:

			T m_data[16];

		};

		typedef Matrix<double, 4, 4> Matrix4d;
		typedef Matrix<float, 4, 4> Matrix4f;
		typedef Matrix<int, 4, 4> Matrix4i;
		typedef Matrix<unsigned int, 4, 4> Matrix4u;

	}

}

#include "Matrix4.inl"