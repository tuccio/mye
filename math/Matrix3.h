#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 3, 3>
		{

		public:

			Matrix(void);
			Matrix(T d);

			inline void Fill(T x);

			inline Matrix<T, 3, 1> GetRow(int i) const;
			inline void SetRow(int i, const Matrix<T, 3, 1> &v);

			inline Matrix<T, 3, 1> GetColumn(int j) const;
			inline void SetColumn(int j, const Matrix<T, 3, 1> &v);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline Matrix<T, 3, 3> operator* (const Matrix<T, 3, 3> &a) const;

			inline T* Data(void);
			inline const T* Data(void) const;

		private:

			T m_data[9];

		};

	}

}

#include "Matrix3.inl"