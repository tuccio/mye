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
			Matrix(T v);
			Matrix(T x, T y);

			inline T& operator[] (int i);
			inline const T& operator[] (int i) const;

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline T Dot(const Matrix<T, 2, 1> &v) const;

			inline bool operator== (const Matrix<T, 2, 1> &b) const;

			inline Matrix<T, 2, 1> operator+ (const Matrix<T, 2, 1> &b) const;
			inline Matrix<T, 2, 1> operator- (const Matrix<T, 2, 1> &b) const;

			inline Matrix<T, 2, 1> operator* (const Matrix<T, 2, 1> &v) const;
			inline Matrix<T, 2, 1> operator/ (const Matrix<T, 2, 1> &v) const;

			inline Matrix<T, 2, 1> operator* (T x) const;

			inline Matrix<T, 2, 1> operator- (void) const;

			inline Matrix<T, 2, 1> Normalize(void) const;
			inline T Length(void) const;

			inline Matrix<T, 2, 1> CwiseAbs(void) const;
			inline Matrix<T, 2, 1> CwiseMin(const Matrix<T, 2, 1> &v) const;
			inline Matrix<T, 2, 1> CwiseMax(const Matrix<T, 2, 1> &v) const;

			inline Matrix<T, 2, 1> Clamp(T minimum, T maximum);
			inline Matrix<T, 2, 1> Clamp(const Matrix<T, 2, 1> &minimum, const Matrix<T, 2, 1> &maximum);

			inline T* Data(void);
			inline const T* Data(void) const;

#include "SwizzleVector2.h"

		private:

			T m_data[2];

		};		

	}

}

#include "Vector2.inl"