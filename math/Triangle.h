#pragma once

#include "SSE.h"

namespace mye
{

	namespace math
	{

		template <typename T>
		class __MYE_MATH_SSE_ALIGNED(16) Triangle
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			inline Matrix<T, 3, 1>& v0(void);
			inline const Matrix<T, 3, 1>& v0(void) const;

			inline Matrix<T, 3, 1>& v1(void);
			inline const Matrix<T, 3, 1>& v1(void) const;

			inline Matrix<T, 3, 1>& v2(void);
			inline const Matrix<T, 3, 1>& v2(void) const;

			inline bool operator== (const Triangle<T> &t);

		private:

			Matrix<T, 3, 1> m_v[3];

		};

		typedef Triangle<float> Trianglef;
		typedef Triangle<double> Triangled;

	}

}

#include "Triangle.inl"