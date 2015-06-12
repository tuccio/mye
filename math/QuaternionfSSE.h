#pragma once

#ifdef MYE_USE_SSE

#include "Quaternion.h"
#include "SSE.h"

namespace mye
{

	namespace math
	{

		template <>
		class __MYE_MATH_SSE_ALIGNED(16) QuaternionTempl<float>
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			__MYE_MATH_INLINE QuaternionTempl(void);
			__MYE_MATH_INLINE QuaternionTempl(float w, float x, float y, float z);
			__MYE_MATH_INLINE QuaternionTempl(const Matrix<float, 3, 1> & axis, float angle);
			__MYE_MATH_INLINE QuaternionTempl(const Matrix<float, 3, 3> & A);

			__MYE_MATH_INLINE QuaternionTempl<float> Inverse(void) const;
			__MYE_MATH_INLINE QuaternionTempl<float> Conjugate(void) const;

			__MYE_MATH_INLINE Matrix<float, 3, 1> Rotate(const Matrix<float, 3, 1> & p) const;

			__MYE_MATH_INLINE float Norm(void) const;
			__MYE_MATH_INLINE QuaternionTempl<float> Normalize(void) const;

			__MYE_MATH_INLINE QuaternionTempl<float> operator+ (const QuaternionTempl<float> & q) const;
			__MYE_MATH_INLINE QuaternionTempl<float> operator- (const QuaternionTempl<float> & q) const;
			__MYE_MATH_INLINE QuaternionTempl<float> operator* (const QuaternionTempl<float> & q) const;

			__MYE_MATH_INLINE float &       operator[] (int i);
			__MYE_MATH_INLINE const float & operator[] (int i) const;

			__MYE_MATH_INLINE float &       x(void);
			__MYE_MATH_INLINE const float & x(void) const;

			__MYE_MATH_INLINE float &       y(void);
			__MYE_MATH_INLINE const float & y(void) const;

			__MYE_MATH_INLINE float &       z(void);
			__MYE_MATH_INLINE const float & z(void) const;

			__MYE_MATH_INLINE float &       w(void);
			__MYE_MATH_INLINE const float & w(void) const;

		private:

			union
			{
				float  __MYE_MATH_SSE_ALIGNED(16) m_data[4];
				__m128                            m_vector;
			};

			__MYE_MATH_INLINE QuaternionTempl(__m128 v);

		};

	}

}

#include "QuaternionfSSE.inl"

#endif