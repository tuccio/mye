#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Transform
		{

		public:

			static Transform<T> Identity(void);

			Transform(void);

			Transform(const Matrix<T, 3, 1> &position,
				const Quaternion<T> &orientation,
				const Matrix<T, 3, 1> &scale);

			~Transform(void);

			inline const Quaternion<T>& GetOrientation(void) const;
			inline void SetOrientation(const Quaternion<T> &q);

			inline const Matrix<T, 3, 1>& GetPosition(void) const;
			inline void SetPosition(const Matrix<T, 3, 1> &position);

			inline const Matrix<T, 3, 1>& GetScale(void) const;
			inline void SetScale(const Matrix<T, 3, 1> &scale);

			inline Matrix<T, 4, 4> GetSRTMatrix(void) const;

			inline Transform<T> Combine(const Transform<T> &transform);

		private:

			Quaternion<T> m_orientation;
			Matrix<T, 3, 1> m_position;
			Matrix<T, 3, 1> m_scale;

		};

	}

}

#include "Transform.inl"