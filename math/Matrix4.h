#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 4, 4>
		{

		public:

			Matrix(void);
			Matrix(T d);

			inline void Fill(T x);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline Matrix<T, 4, 4> operator* (const Matrix<T, 4, 4> &a) const;

			inline T* Data(void);
			inline const T* Data(void) const;

		private:

			T m_data[16];

		};

	}

}

#include "Matrix4.inl"