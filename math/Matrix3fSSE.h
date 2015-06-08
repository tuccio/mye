#pragma once

#ifdef MYE_USE_SSE

#include "Matrix.h"
#include "Matrix3.h"
#include "SSE.h"


#define __MYE_MATH_MATRIX3_DEFINE_MEMBER_ACCESS(TYPE) BOOST_PP_REPEAT(9, __MYE_MATH_DEFINE_MATRIX3_GET_SET, TYPE)\
                                                      BOOST_PP_REPEAT(9, __MYE_MATH_DEFINE_MATRIX3_REFS, TYPE)

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

			__MYE_MATH_INLINE Matrix<float, 3, 3> Transpose(void) const;
			__MYE_MATH_INLINE Matrix<float, 3, 3> Inverse(void) const;

			__MYE_MATH_INLINE float Determinant(void) const;

			__MYE_MATH_INLINE float *       Data(void);
			__MYE_MATH_INLINE const float * Data(void) const;

			__MYE_MATH_MATRIX3_DEFINE_MEMBER_ACCESS(float)

		private:

			union
			{
				float  __MYE_MATH_SSE_ALIGNED(16) m_data[12];
				__m128                            m_vector[3];
			};

			template <typename T, int N, int M>
			friend class Matrix;

		};

	}

}

#endif