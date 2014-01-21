#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 3, 1>
		{

		public:

			Matrix(void);
			Matrix(T v);
			Matrix(T x, T y, T z);
			Matrix(const Matrix<T, 2, 1> &v, T z);

			inline T& operator[] (int i);
			inline const T& operator[] (int i) const;

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline T Dot(const Matrix<T, 3, 1> &v) const;
			inline Matrix<T, 3, 1> Cross(const Matrix<T, 3, 1> &v) const;

			inline Matrix<T, 3, 1> operator+ (const Matrix<T, 3, 1> &b) const;
			inline Matrix<T, 3, 1> operator- (const Matrix<T, 3, 1> &b) const;

			inline Matrix<T, 3, 1> operator* (const Matrix<T, 3, 1> &v) const;
			inline Matrix<T, 3, 1> operator/ (const Matrix<T, 3, 1> &v) const;

			inline Matrix<T, 3, 1> operator* (T x) const;

			inline Matrix<T, 3, 1> operator- (void) const;

			inline Matrix<T, 3, 1>& Normalize(void);
			inline Matrix<T, 3, 1> Normalized(void) const;
			inline T Length(void) const;

			inline Matrix<T, 3, 1> CwiseAbs(void) const;
			inline Matrix<T, 3, 1> CwiseMin(const Matrix<T, 3, 1> &v) const;
			inline Matrix<T, 3, 1> CwiseMax(const Matrix<T, 3, 1> &v) const;

			inline T& x(void);
			inline const T& x(void) const;

			inline T& y(void);
			inline const T& y(void) const;

			inline T& z(void);
			inline const T& z(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;

		private:

			T m_data[3];

		};

	}

}

#include "Vector3.inl"