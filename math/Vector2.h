#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 2, 1>
		{

		public:

			Matrix(void);
			Matrix(T x, T y);

			inline T& operator[] (int i);
			inline const T& operator[] (int i) const;

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline T Dot(const Matrix<T, 2, 1> &v) const;

			inline Matrix<T, 2, 1> operator+ (const Matrix<T, 2, 1> &b);
			inline Matrix<T, 2, 1> operator- (const Matrix<T, 2, 1> &b);

			inline Matrix<T, 2, 1> Normalize(void) const;
			inline T Length(void) const;

			inline T& x(void);
			inline const T& x(void) const;

			inline T& y(void);
			inline const T& y(void) const;

			inline T& u(void);
			inline const T& u(void) const;

			inline T& v(void);
			inline const T& v(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;

		private:

			T m_data[2];

		};

		typedef Matrix<double, 2, 1> Vector2d;
		typedef Matrix<float, 2, 1> Vector2f;
		typedef Matrix<int, 2, 1> Vector2i;
		typedef Matrix<unsigned int, 2, 1> Vector2u;

	}

}

#include "Vector2.inl"