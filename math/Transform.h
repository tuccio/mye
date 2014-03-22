#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Transformt
		{

		public:

			static Transformt<T> Identity(void);

			Transformt(void);

			Transformt(const Matrix<T, 3, 1> &position,
				const Quaterniont<T> &orientation,
				const Matrix<T, 3, 1> &scale);

			~Transformt(void);

			inline const Quaterniont<T>& GetOrientation(void) const;
			inline void SetOrientation(const Quaterniont<T> &q);

			inline const Matrix<T, 3, 1>& GetPosition(void) const;
			inline void SetPosition(const Matrix<T, 3, 1> &position);

			inline const Matrix<T, 3, 1>& GetScale(void) const;
			inline void SetScale(const Matrix<T, 3, 1> &scale);

			inline Matrix<T, 4, 4> GetSRTMatrix(void) const;			

		private:

			Quaterniont<T> m_orientation;
			Matrix<T, 3, 1> m_position;
			Matrix<T, 3, 1> m_scale;

		};

	}

}

#include "Transform.inl"
