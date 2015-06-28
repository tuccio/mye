#pragma once

#include "SSE.h"

namespace mye
{

	namespace math
	{

		template <typename T>
		class __MYE_MATH_SSE_ALIGNED(16) SphereTempl
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			__MYE_MATH_INLINE SphereTempl(void);
			__MYE_MATH_INLINE SphereTempl(const Matrix<T, 3, 1> & center, T radius);

			__MYE_MATH_INLINE const Matrix<T, 3, 1> GetCenter(void) const;
			__MYE_MATH_INLINE void                  SetCenter(const Matrix<T, 3, 1> & center);

			__MYE_MATH_INLINE T    GetRadius(void) const;
			__MYE_MATH_INLINE void SetRadius(T radius);

		private:

			Matrix<T, 4, 1> m_sphere;

		};

	}

}

#include "Sphere.inl"