#pragma once

#ifdef MYE_USE_SSE

#include "Matrix.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "SSE.h"

#define __MYE_MATH_DEFINE_MATRIX3_SSE_INDEX(I) BOOST_PP_ADD(BOOST_PP_MUL(4, BOOST_PP_DIV(I, 4)), BOOST_PP_MOD(I, 4))

#define __MYE_MATH_DEFINE_MATRIX3_SSE_GET_SET(N, I, TYPE) \
	BOOST_PP_EXPR_IF(BOOST_PP_NOT_EQUAL(BOOST_PP_MOD(I, 4), 3),\
		__MYE_MATH_INLINE TYPE BOOST_PP_CAT(Get, BOOST_PP_CAT(BOOST_PP_DIV(I, 4), BOOST_PP_MOD(I, 4))) (void) const { return *(m_data + __MYE_MATH_DEFINE_MATRIX3_SSE_INDEX(I)); }\
		__MYE_MATH_INLINE void BOOST_PP_CAT(Set, BOOST_PP_CAT(BOOST_PP_DIV(I, 4), BOOST_PP_MOD(I, 4))) (const TYPE & x) { *(m_data + __MYE_MATH_DEFINE_MATRIX3_SSE_INDEX(I)) = x; })

#define __MYE_MATH_DEFINE_MATRIX3_SSE_REFS(N, I, TYPE) \
	BOOST_PP_EXPR_IF(BOOST_PP_NOT_EQUAL(BOOST_PP_MOD(I, 4), 3),\
		__MYE_MATH_INLINE const TYPE & BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 4), BOOST_PP_MOD(I, 4))) (void) const { return *(m_data + I); }\
		__MYE_MATH_INLINE TYPE &       BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 4), BOOST_PP_MOD(I, 4))) (void) { return *(m_data + __MYE_MATH_DEFINE_MATRIX3_SSE_INDEX(I)); })

#define __MYE_MATH_MATRIX3_DEFINE_MEMBER_ACCESS_SSE   BOOST_PP_REPEAT(12, __MYE_MATH_DEFINE_MATRIX3_SSE_GET_SET, float)\
                                                      BOOST_PP_REPEAT(12, __MYE_MATH_DEFINE_MATRIX3_SSE_REFS, float)

namespace mye
{

	namespace math
	{

		template <>
		class Matrix<float, 3, 3>
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			Matrix(void);
			Matrix(const Matrix<float, 4, 4> & m);
			Matrix(float d);

			__MYE_MATH_INLINE void Fill(float x);

			__MYE_MATH_INLINE Matrix<float, 3, 1> GetRow(int i) const;
			__MYE_MATH_INLINE void SetRow(int i, const Matrix<float, 3, 1> & v);

			__MYE_MATH_INLINE Matrix<float, 3, 1> GetColumn(int j) const;
			__MYE_MATH_INLINE void SetColumn(int j, const Matrix<float, 3, 1> & v);

			__MYE_MATH_INLINE float &       operator() (int i, int j);
			__MYE_MATH_INLINE const float & operator() (int i, int j) const;

			__MYE_MATH_INLINE Matrix<float, 3, 3> operator- (void) const;

			__MYE_MATH_INLINE Matrix<float, 3, 3> & operator= (const Matrix<float, 3, 3> & a);

			__MYE_MATH_INLINE Matrix<float, 3, 3> operator+ (const Matrix<float, 3, 3> & a) const;
			__MYE_MATH_INLINE Matrix<float, 3, 3> operator- (const Matrix<float, 3, 3> & a) const;

			__MYE_MATH_INLINE Matrix<float, 3, 3> operator* (const Matrix<float, 3, 3> & a) const;

			__MYE_MATH_INLINE Matrix<float, 3, 3> operator* (float s) const;
			__MYE_MATH_INLINE Matrix<float, 3, 3> operator/ (float s) const;

			__MYE_MATH_INLINE bool operator== (const Matrix<float, 3, 3> & b) const;

			__MYE_MATH_INLINE Matrix<float, 3, 3> Transpose(void) const;
			__MYE_MATH_INLINE Matrix<float, 3, 3> Inverse(void) const;

			__MYE_MATH_INLINE float Determinant(void) const;

			__MYE_MATH_INLINE float *       Data(void);
			__MYE_MATH_INLINE const float * Data(void) const;

			__MYE_MATH_MATRIX3_DEFINE_MEMBER_ACCESS_SSE

		private:

			union
			{
				float  __MYE_MATH_SSE_ALIGNED(16) m_data[12];
				__m128                            m_vector[3];
			};

			template <typename T, int N, int M>
			friend class Matrix;

			template <typename T>
			friend class QuaternionTempl;

		};

	}

}

#endif