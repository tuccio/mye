#pragma once

#ifdef MYE_USE_SSE

#include "Matrix.h"
#include "Matrix3.h"
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

		template <>
		class __MYE_MATH_SSE_ALIGNED(16) Matrix<float, 2, 2>
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			Matrix(void);
			Matrix(float d);

			__MYE_MATH_INLINE void Fill(float x);

			__MYE_MATH_INLINE Matrix<float, 2, 1> GetRow(int i) const;
			__MYE_MATH_INLINE void SetRow(int i, const Matrix<float, 2, 1> & v);

			__MYE_MATH_INLINE Matrix<float, 2, 1> GetColumn(int j) const;
			__MYE_MATH_INLINE void SetColumn(int j, const Matrix<float, 2, 1> & v);

			__MYE_MATH_INLINE float &       operator() (int i, int j);
			__MYE_MATH_INLINE const float & operator() (int i, int j) const;

			__MYE_MATH_INLINE Matrix<float, 2, 2> operator- (void) const;

			__MYE_MATH_INLINE Matrix<float, 2, 2> & operator= (const Matrix<float, 2, 2> & a);

			__MYE_MATH_INLINE Matrix<float, 2, 2> operator+ (const Matrix<float, 2, 2> & a) const;
			__MYE_MATH_INLINE Matrix<float, 2, 2> operator- (const Matrix<float, 2, 2> & a) const;

			__MYE_MATH_INLINE Matrix<float, 2, 2> operator* (const Matrix<float, 2, 2> & a) const;

			__MYE_MATH_INLINE Matrix<float, 2, 2> operator* (float s) const;
			__MYE_MATH_INLINE Matrix<float, 2, 2> operator/ (float s) const;

			__MYE_MATH_INLINE Matrix<float, 2, 2> Transpose(void) const;
			__MYE_MATH_INLINE Matrix<float, 2, 2> Inverse(void) const;

			__MYE_MATH_INLINE float Determinant(void) const;

			__MYE_MATH_INLINE float *       Data(void);
			__MYE_MATH_INLINE const float * Data(void) const;

			__MYE_MATH_MATRIX2_DEFINE_MEMBER_ACCESS(float)

		private:

			__MYE_MATH_INLINE Matrix(__m128 v);

			union
			{
				float  __MYE_MATH_SSE_ALIGNED(16) m_data[4];
				__m128                            m_vector;
			};

			template <typename T, int N, int M>
			friend class Matrix;

		};

	}

}

#endif