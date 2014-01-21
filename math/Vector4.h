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
			Matrix(T x, T y, T z, T w);

			inline T& operator[] (int i);
			inline const T& operator[] (int i) const;

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline T Dot(const Matrix<T, 4, 1> &v) const;

			inline Matrix<T, 4, 1> operator+ (const Matrix<T, 4, 1> &b);
			inline Matrix<T, 4, 1> operator- (const Matrix<T, 4, 1> &b);

			inline Matrix<T, 4, 1> Normalize(void) const;
			inline T Length(void) const;

			inline T& x(void);
			inline const T& x(void) const;

			inline T& y(void);
			inline const T& y(void) const;

			inline T& z(void);
			inline const T& z(void) const;

			inline T& w(void);
			inline const T& w(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;
			
		private:

			T m_data[4];

		};

		typedef Matrix<double, 4, 1> Vector4d;
		typedef Matrix<float, 4, 1> Vector4f;
		typedef Matrix<int, 4, 1> Vector4i;
		typedef Matrix<unsigned int, 4, 1> Vector4u;

	}

}

#include "Vector4.inl"