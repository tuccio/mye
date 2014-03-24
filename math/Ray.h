#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class RayTempl
		{

		public:

			RayTempl(void);
			~RayTempl(void);

			inline Matrix<T, 3, 1>& Origin(void);
			inline const Matrix<T, 3, 1>& Origin(void) const;
			inline const Matrix<T, 3, 1>& GetOrigin(void) const;
			inline void SetOrigin(const Matrix<T, 3, 1> &origin);

			inline Matrix<T, 3, 1>& Direction(void);
			inline const Matrix<T, 3, 1>& Direction(void) const;
			inline const Matrix<T, 3, 1>& GetDirection(void) const;
			inline void SetDirection(const Matrix<T, 3, 1> &direction);

			inline Matrix<T, 3, 1> At(T t) const;
			
		private:

			Matrix<T, 3, 1> m_origin;
			Matrix<T, 3, 1> m_direction;

		};

	}

}

#include "Ray.inl"
