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

			inline Matrix<T, 3, 3>& ScaleInPlace(T s);

			inline Matrix<T, 3, 3> Transpose(void) const;
			inline T Determinant(void) const;
			inline Matrix<T, 3, 3> Inverse(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;

			inline T& m00(void);
			inline const T& m00(void) const;

			inline T& m01(void);
			inline const T& m01(void) const;

			inline T& m02(void);
			inline const T& m02(void) const;

			inline T& m10(void);
			inline const T& m10(void) const;

			inline T& m11(void);
			inline const T& m11(void) const;

			inline T& m12(void);
			inline const T& m12(void) const;

			inline T& m20(void);
			inline const T& m20(void) const;

			inline T& m21(void);
			inline const T& m21(void) const;

			inline T& m22(void);
			inline const T& m22(void) const;

		private:

			T m_data[9];

		};

	}

}

#include "Matrix3.inl"