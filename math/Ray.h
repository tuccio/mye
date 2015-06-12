#pragma once

#include "SSE.h"

namespace mye
{

	namespace math
	{

		template <typename T>
		class __MYE_MATH_SSE_ALIGNED(16) RayTempl
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			RayTempl(void);
			RayTempl(const Matrix<T, 3, 1> &origin, const Matrix<T, 3, 1> &direction);

			~RayTempl(void);

			inline Matrix<T, 3, 1>& Origin(void);
			inline const Matrix<T, 3, 1>& Origin(void) const;
			inline const Matrix<T, 3, 1>& GetOrigin(void) const;
			inline void SetOrigin(const Matrix<T, 3, 1> &origin);

			inline Matrix<T, 3, 1>& Direction(void);
			inline const Matrix<T, 3, 1>& Direction(void) const;
			inline const Matrix<T, 3, 1>& GetDirection(void) const;
			inline void SetDirection(const Matrix<T, 3, 1> &direction);

			inline Matrix<T, 3, 1> Evaluate(T t) const;
			
		private:

			Matrix<T, 3, 1> m_origin;
			Matrix<T, 3, 1> m_direction;

		};

	}

}

#include "Ray.inl"
