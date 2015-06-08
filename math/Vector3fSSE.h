#pragma once

#ifdef MYE_USE_SSE

#include "SSE.h"

namespace mye
{

	namespace math
	{

		template <>
		class __MYE_MATH_SSE_ALIGNED(16) Matrix<float, 3, 1>
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			__MYE_MATH_INLINE Matrix(void);
			__MYE_MATH_INLINE Matrix(float v);
			__MYE_MATH_INLINE Matrix(float x, float y, float z);
			__MYE_MATH_INLINE Matrix(const Matrix<float, 2, 1> & v, float z);

			__MYE_MATH_INLINE float &       operator[] (int i);
			__MYE_MATH_INLINE const float & operator[] (int i) const;

			__MYE_MATH_INLINE float &       operator() (int i, int j);
			__MYE_MATH_INLINE const float & operator() (int i, int j) const;

			__MYE_MATH_INLINE float Dot(const Matrix<float, 3, 1> & v) const;
			__MYE_MATH_INLINE Matrix<float, 3, 1> Cross(const Matrix<float, 3, 1> & v) const;

			__MYE_MATH_INLINE bool operator == (const Matrix<float, 3, 1> & b) const;
			__MYE_MATH_INLINE bool operator != (const Matrix<float, 3, 1> & b) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> operator+ (const Matrix<float, 3, 1> & b) const;
			__MYE_MATH_INLINE Matrix<float, 3, 1> operator- (const Matrix<float, 3, 1> & b) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> operator* (const Matrix<float, 3, 1> & b) const;
			__MYE_MATH_INLINE Matrix<float, 3, 1> operator/ (const Matrix<float, 3, 1> & b) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> operator* (float x) const;
			__MYE_MATH_INLINE Matrix<float, 3, 1> operator/ (float x) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> operator- (void) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> & operator= (const Matrix<float, 3, 1> & v);

			__MYE_MATH_INLINE Matrix<float, 3, 1> Normalize(void) const;
			__MYE_MATH_INLINE Matrix<float, 3, 1> NormalizeFast(void) const;

			__MYE_MATH_INLINE float Length(void) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> Reflect(const Matrix<float, 3, 1> & normal) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> CwiseAbs(void) const;
			__MYE_MATH_INLINE Matrix<float, 3, 1> CwiseMin(const Matrix<float, 3, 1> & v) const;
			__MYE_MATH_INLINE Matrix<float, 3, 1> CwiseMax(const Matrix<float, 3, 1> & v) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> Clamp(float minimum, float maximum);
			__MYE_MATH_INLINE Matrix<float, 3, 1> Clamp(const Matrix<float, 3, 1> & minimum, const Matrix<float, 3, 1> & maximum);

			__MYE_MATH_INLINE float *       Data(void);
			__MYE_MATH_INLINE const float * Data(void) const;

#include "SwizzleVector3f.h"

		private:

			__MYE_MATH_INLINE Matrix(__m128 v);

			union
			{
				float  __MYE_MATH_SSE_ALIGNED(16) m_data[3];
				__m128                            m_vector;
			};

			template <typename T, int N, int M>
			friend class Matrix;

		};

	}

}

#endif