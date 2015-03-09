#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class TransformTempl
		{

		public:

			static TransformTempl<T> Identity(void);

			TransformTempl(void);

			TransformTempl(const Matrix<T, 3, 1> &position,
				const QuaternionTempl<T> &orientation,
				const Matrix<T, 3, 1> &scale);

			~TransformTempl(void);

			inline const QuaternionTempl<T>& GetOrientation(void) const;
			inline void SetOrientation(const QuaternionTempl<T> &q);

			inline QuaternionTempl<T>& Orientation(void);

			inline const Matrix<T, 3, 1>& GetPosition(void) const;
			inline void SetPosition(const Matrix<T, 3, 1> &position);

			inline Matrix<T, 3, 1>& Position(void);

			inline const Matrix<T, 3, 1>& GetScale(void) const;
			inline void SetScale(const Matrix<T, 3, 1> &scale);

			inline Matrix<T, 3, 1>& Scale(void);

			inline Matrix<T, 4, 4> GetSRTMatrix(void) const;			

		private:

			QuaternionTempl<T> m_orientation;
			Matrix<T, 3, 1> m_position;
			Matrix<T, 3, 1> m_scale;

		};

	}

}

#include "Transform.inl"
