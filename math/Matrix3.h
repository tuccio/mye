#pragma once

#include "SSE.h"

#define __MYE_MATH_DEFINE_MATRIX3_GET_SET(N, I, TYPE) \
	__MYE_MATH_INLINE TYPE BOOST_PP_CAT(Get, BOOST_PP_CAT(BOOST_PP_DIV(I, 3), BOOST_PP_MOD(I, 3))) (void) const { return *(m_data + I); }\
	__MYE_MATH_INLINE void BOOST_PP_CAT(Set, BOOST_PP_CAT(BOOST_PP_DIV(I, 3), BOOST_PP_MOD(I, 3))) (const TYPE & x) { *(m_data + I) = x; }

#define __MYE_MATH_DEFINE_MATRIX3_REFS(N, I, TYPE) \
	__MYE_MATH_INLINE const TYPE & BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 3), BOOST_PP_MOD(I, 3))) (void) const { return *(m_data + I); }\
	__MYE_MATH_INLINE TYPE & BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 3), BOOST_PP_MOD(I, 3))) (void) { return *(m_data + I); }

#define __MYE_MATH_MATRIX3_DEFINE_MEMBER_ACCESS(TYPE) BOOST_PP_REPEAT(9, __MYE_MATH_DEFINE_MATRIX3_GET_SET, TYPE)\
                                                      BOOST_PP_REPEAT(9, __MYE_MATH_DEFINE_MATRIX3_REFS, TYPE)

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 3, 3>
		{

		public:

			Matrix(void);
			Matrix(const Matrix<T, 4, 4> & m);
			Matrix(T d);

			__MYE_MATH_INLINE void Fill(T x);

			__MYE_MATH_INLINE Matrix<T, 3, 1> GetRow(int i) const;
			__MYE_MATH_INLINE void SetRow(int i, const Matrix<T, 3, 1> & v);

			__MYE_MATH_INLINE Matrix<T, 3, 1> GetColumn(int j) const;
			__MYE_MATH_INLINE void SetColumn(int j, const Matrix<T, 3, 1> & v);

			__MYE_MATH_INLINE T &       operator() (int i, int j);
			__MYE_MATH_INLINE const T & operator() (int i, int j) const;

			__MYE_MATH_INLINE Matrix<T, 3, 3> operator- (void) const;

			__MYE_MATH_INLINE Matrix<T, 3, 3> & operator= (const Matrix<T, 3, 3> & a);

			__MYE_MATH_INLINE Matrix<T, 3, 3> operator+ (const Matrix<T, 3, 3> & a) const;
			__MYE_MATH_INLINE Matrix<T, 3, 3> operator- (const Matrix<T, 3, 3> & a) const;

			__MYE_MATH_INLINE Matrix<T, 3, 3> operator* (const Matrix<T, 3, 3> & a) const;

			__MYE_MATH_INLINE Matrix<T, 3, 3> operator* (T s) const;
			__MYE_MATH_INLINE Matrix<T, 3, 3> operator/ (T s) const;

			__MYE_MATH_INLINE Matrix<T, 3, 3> Transpose(void) const;
			__MYE_MATH_INLINE Matrix<T, 3, 3> Inverse(void) const;

			__MYE_MATH_INLINE T Determinant(void) const;
			
			__MYE_MATH_INLINE T *       Data(void);
			__MYE_MATH_INLINE const T * Data(void) const;

			__MYE_MATH_MATRIX3_DEFINE_MEMBER_ACCESS(T)

		private:

			T m_data[9];

		};

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 3, 3> operator* (T s, const Matrix<T, 3, 3> & m);

	}

}

#include "Matrix3.inl"