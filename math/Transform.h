#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class __MYE_MATH_SSE_ALIGNED(16) TransformTempl
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			static TransformTempl<T> Identity(void);

			TransformTempl(void);

			TransformTempl(const Matrix<T, 3, 1>    & position,
			               const QuaternionTempl<T> & orientation,
			               const Matrix<T, 3, 1>    & scale);

			~TransformTempl(void);

			__MYE_MATH_INLINE const QuaternionTempl<T> & GetOrientation(void) const;
			__MYE_MATH_INLINE void SetOrientation(const QuaternionTempl<T> & q);

			__MYE_MATH_INLINE QuaternionTempl<T> & Orientation(void);

			__MYE_MATH_INLINE const Matrix<T, 3, 1> & GetPosition(void) const;
			__MYE_MATH_INLINE void SetPosition(const Matrix<T, 3, 1> & position);

			__MYE_MATH_INLINE Matrix<T, 3, 1> & Position(void);

			__MYE_MATH_INLINE const Matrix<T, 3, 1> & GetScale(void) const;
			__MYE_MATH_INLINE void SetScale(const Matrix<T, 3, 1> & scale);

			__MYE_MATH_INLINE Matrix<T, 3, 1> & Scale(void);

			__MYE_MATH_INLINE Matrix<T, 4, 4> GetSRTMatrix(void) const;

		private:

			QuaternionTempl<T> m_orientation;
			Matrix<T, 3, 1>    m_position;
			Matrix<T, 3, 1>    m_scale;

		};

	}

}

#include "Transform.inl"
