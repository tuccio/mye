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
			Matrix(T d);
			Matrix(const Matrix<T, 3, 3> &m);

			inline void Fill(T x);

			inline Matrix<T, 4, 1> GetRow(int i) const;
			inline void SetRow(int i, const Matrix<T, 4, 1> &v);

			inline Matrix<T, 4, 1> GetColumn(int i) const;
			inline void SetColumn(int i, const Matrix<T, 4, 1> &v);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline Matrix<T, 4, 4>& ScaleInPlace(T s);

			inline Matrix<T, 4, 4> operator* (const Matrix<T, 4, 4> &a) const;
			inline Matrix<T, 4, 4> operator* (T s) const;

			inline Matrix<T, 4, 4> Transpose(void) const;
			inline Matrix<T, 4, 4> Inverse(void) const;
			inline T Determinant(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;

		private:

			T m_data[16];

		};

	}

}

#include "Matrix4.inl"