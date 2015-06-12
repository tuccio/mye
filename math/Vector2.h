#pragma once

#include "SSE.h"

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 2, 1>
		{

		public:

			__MYE_MATH_INLINE Matrix(void);
			__MYE_MATH_INLINE Matrix(T v);
			__MYE_MATH_INLINE Matrix(T x, T y);

			__MYE_MATH_INLINE T & operator[] (int i);
			__MYE_MATH_INLINE const T & operator[] (int i) const;

			__MYE_MATH_INLINE T & operator() (int i, int j);
			__MYE_MATH_INLINE const T & operator() (int i, int j) const;

			__MYE_MATH_INLINE T Dot(const Matrix<T, 2, 1> & v) const;

			__MYE_MATH_INLINE bool operator== (const Matrix<T, 2, 1> & b) const;
			__MYE_MATH_INLINE bool operator!= (const Matrix<T, 2, 1> & b) const;

			__MYE_MATH_INLINE Matrix<T, 2, 1> operator+ (const Matrix<T, 2, 1> & b) const;
			__MYE_MATH_INLINE Matrix<T, 2, 1> operator- (const Matrix<T, 2, 1> & b) const;

			__MYE_MATH_INLINE Matrix<T, 2, 1> operator* (const Matrix<T, 2, 1> & b) const;
			__MYE_MATH_INLINE Matrix<T, 2, 1> operator/ (const Matrix<T, 2, 1> & b) const;

			__MYE_MATH_INLINE Matrix<T, 2, 1> operator* (T x) const;
			__MYE_MATH_INLINE Matrix<T, 2, 1> operator/ (T x) const;

			__MYE_MATH_INLINE Matrix<T, 2, 1> operator- (void) const;

			__MYE_MATH_INLINE Matrix<T, 2, 1> & operator= (const Matrix<T, 2, 1> & b);

			__MYE_MATH_INLINE Matrix<T, 2, 1> Normalize(void) const;
			__MYE_MATH_INLINE Real Length(void) const;

			__MYE_MATH_INLINE Matrix<T, 2, 1> CwiseAbs(void) const;
			__MYE_MATH_INLINE Matrix<T, 2, 1> CwiseMin(const Matrix<T, 2, 1> &v) const;
			__MYE_MATH_INLINE Matrix<T, 2, 1> CwiseMax(const Matrix<T, 2, 1> &v) const;

			__MYE_MATH_INLINE Matrix<T, 2, 1> Clamp(T minimum, T maximum);
			__MYE_MATH_INLINE Matrix<T, 2, 1> Clamp(const Matrix<T, 2, 1> & minimum, const Matrix<T, 2, 1> & maximum);

			__MYE_MATH_INLINE T *       Data(void);
			__MYE_MATH_INLINE const T * Data(void) const;


#include "SwizzleVector2T.h"

		private:

			T m_data[2];

		};		

	}

}

#include "Vector2.inl"