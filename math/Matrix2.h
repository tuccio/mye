#pragma once

#include "SSE.h"

#define __MYE_MATH_DEFINE_MATRIX2_GET_SET(N, I, TYPE) \
	__MYE_MATH_INLINE TYPE BOOST_PP_CAT(Get, BOOST_PP_CAT(BOOST_PP_DIV(I, 2), BOOST_PP_MOD(I, 2))) (void) const { return *(m_data + I); }\
	__MYE_MATH_INLINE void BOOST_PP_CAT(Set, BOOST_PP_CAT(BOOST_PP_DIV(I, 2), BOOST_PP_MOD(I, 2))) (const TYPE & x) { *(m_data + I) = x; }

#define __MYE_MATH_DEFINE_MATRIX2_REFS(N, I, TYPE) \
	__MYE_MATH_INLINE const TYPE & BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 2), BOOST_PP_MOD(I, 2))) (void) const { return *(m_data + I); }\
	__MYE_MATH_INLINE TYPE & BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 2), BOOST_PP_MOD(I, 2))) (void) { return *(m_data + I); }

#define __MYE_MATH_MATRIX2_DEFINE_MEMBER_ACCESS(TYPE) BOOST_PP_REPEAT(4, __MYE_MATH_DEFINE_MATRIX2_GET_SET, TYPE)\
                                                      BOOST_PP_REPEAT(4, __MYE_MATH_DEFINE_MATRIX2_REFS, TYPE)

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 2, 2>
		{

		public:

			Matrix(void);
			Matrix(T d);

			__MYE_MATH_INLINE void Fill(T x);

			__MYE_MATH_INLINE Matrix<T, 2, 1> GetRow(int i) const;
			__MYE_MATH_INLINE void SetRow(int i, const Matrix<T, 2, 1> & v);

			__MYE_MATH_INLINE Matrix<T, 2, 1> GetColumn(int j) const;
			__MYE_MATH_INLINE void SetColumn(int j, const Matrix<T, 2, 1> & v);

			__MYE_MATH_INLINE T &       operator() (int i, int j);
			__MYE_MATH_INLINE const T & operator() (int i, int j) const;

			__MYE_MATH_INLINE Matrix<T, 2, 2> operator- (void) const;

			__MYE_MATH_INLINE Matrix<T, 2, 2> & operator= (const Matrix<T, 2, 2> & a);

			__MYE_MATH_INLINE Matrix<T, 2, 2> operator+ (const Matrix<T, 2, 2> & a) const;
			__MYE_MATH_INLINE Matrix<T, 2, 2> operator- (const Matrix<T, 2, 2> & a) const;

			__MYE_MATH_INLINE Matrix<T, 2, 2> operator* (const Matrix<T, 2, 2> & a) const;

			__MYE_MATH_INLINE Matrix<T, 2, 2> operator* (T s) const;
			__MYE_MATH_INLINE Matrix<T, 2, 2> operator/ (T s) const;

			__MYE_MATH_INLINE Matrix<T, 2, 2> Transpose(void) const;
			__MYE_MATH_INLINE Matrix<T, 2, 2> Inverse(void) const;

			__MYE_MATH_INLINE T Determinant(void) const;

			__MYE_MATH_INLINE T *       Data(void);
			__MYE_MATH_INLINE const T * Data(void) const;

			__MYE_MATH_MATRIX2_DEFINE_MEMBER_ACCESS(T)

		private:

			T m_data[4];

		};

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> operator* (T s, const Matrix<T, 2, 2> & m);

	}

}

#include "Matrix2.inl"