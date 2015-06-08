#pragma once

#ifdef MYE_USE_SSE

#include "Matrix.h"
#include "Matrix4.h"
#include "SSE.h"

#include <initializer_list>

namespace mye
{

	namespace math
	{

		template <>
		class __MYE_MATH_SSE_ALIGNED(16) Matrix<float, 4, 4>
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			__MYE_MATH_INLINE Matrix(void);
			__MYE_MATH_INLINE Matrix(float d);
			__MYE_MATH_INLINE Matrix(const Matrix<float, 3, 3> & m);
			__MYE_MATH_INLINE Matrix(std::initializer_list<float> initializer);

			__MYE_MATH_INLINE void Fill(float x);

			__MYE_MATH_INLINE Matrix<float, 4, 1> GetRow(int i) const;
			__MYE_MATH_INLINE void SetRow(int i, const Matrix<float, 4, 1> & v);

			__MYE_MATH_INLINE Matrix<float, 4, 1> GetColumn(int i) const;
			__MYE_MATH_INLINE void SetColumn(int i, const Matrix<float, 4, 1> & v);

			__MYE_MATH_INLINE float &       operator() (int i, int j);
			__MYE_MATH_INLINE const float & operator() (int i, int j) const;

			__MYE_MATH_INLINE Matrix<float, 4, 4> & operator= (const Matrix<float, 4, 4> & m);

			__MYE_MATH_INLINE Matrix<float, 4, 4> operator* (const Matrix<float, 4, 4> & a) const;

			__MYE_MATH_INLINE Matrix<float, 4, 4> operator* (float s) const;
			__MYE_MATH_INLINE Matrix<float, 4, 4> operator/ (float s) const;

			__MYE_MATH_INLINE Matrix<float, 4, 4> operator+ (const Matrix<float, 4, 4> & a) const;
			__MYE_MATH_INLINE Matrix<float, 4, 4> operator- (const Matrix<float, 4, 4> & a) const;

			__MYE_MATH_INLINE Matrix<float, 4, 4> operator- (void) const;

			__MYE_MATH_INLINE Matrix<float, 4, 4> Transpose(void) const;
			__MYE_MATH_INLINE Matrix<float, 4, 4> Inverse(void) const;

			__MYE_MATH_INLINE float Determinant(void) const;

			__MYE_MATH_INLINE float *       Data(void);
			__MYE_MATH_INLINE const float * Data(void) const;

			__MYE_MATH_MATRIX4_DEFINE_MEMBER_ACCESS(float)

		private:

			union
			{
				float  __MYE_MATH_SSE_ALIGNED(16) m_data[16];
				__m128                            m_vector[4];
			};

			template <typename T, int N, int M>
			friend class Matrix;

		};

	}

}

#endif