#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 4, 1>
		{

		public:

			Matrix(void);
			Matrix(T v);
			Matrix(const Matrix<T, 3, 1> &v, T w);
			Matrix(T x, T y, T z, T w);

			inline T& operator[] (int i);
			inline const T& operator[] (int i) const;

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline T Dot(const Matrix<T, 4, 1> &v) const;

			inline Matrix<T, 4, 1> operator+ (const Matrix<T, 4, 1> &b) const;
			inline Matrix<T, 4, 1> operator- (const Matrix<T, 4, 1> &b) const;

			inline Matrix<T, 4, 1> operator* (const Matrix<T, 4, 1> &v) const;
			inline Matrix<T, 4, 1> operator/ (const Matrix<T, 4, 1> &v) const;

			inline Matrix<T, 4, 1> operator* (T x) const;

			inline Matrix<T, 4, 1> operator- (void) const;

			inline Matrix<T, 4, 1>& Normalize(void);
			inline Matrix<T, 4, 1> Normalized(void) const;
			inline T Length(void) const;

			inline Matrix<T, 4, 1> CwiseAbs(void) const;
			inline Matrix<T, 4, 1> CwiseMin(const Matrix<T, 4, 1> &v) const;
			inline Matrix<T, 4, 1> CwiseMax(const Matrix<T, 4, 1> &v) const;

			inline T& x(void);
			inline const T& x(void) const;

			inline T& y(void);
			inline const T& y(void) const;

			inline T& z(void);
			inline const T& z(void) const;

			inline T& w(void);
			inline const T& w(void) const;

			inline T& r(void);
			inline const T& r(void) const;

			inline T& g(void);
			inline const T& g(void) const;

			inline T& b(void);
			inline const T& b(void) const;

			inline T& a(void);
			inline const T& a(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;
			
		private:

			T m_data[4];

		};		

	}

}

#include "Vector4.inl"