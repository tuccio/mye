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
			Matrix(const Matrix<T, 3, 3> &m);

			inline void Fill(T x);

			inline Matrix<T, 4, 1> GetRow(int i) const;
			inline void SetRow(int i, const Matrix<T, 4, 1> &v);

			inline Matrix<T, 4, 1> GetColumn(int i) const;
			inline void SetColumn(int i, const Matrix<T, 4, 1> &v);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline Matrix<T, 4, 4>& ScaleInPlace(T s);

			inline Matrix<T, 4, 4> operator* (const Matrix<T, 4, 4> &a) const;
			inline Matrix<T, 4, 4> operator* (T s) const;

			inline Matrix<T, 4, 4> Transpose(void) const;
			inline Matrix<T, 4, 4> Inverse(void) const;
			inline T Determinant(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;

			inline T& m00(void);
			inline const T& m00(void) const;

			inline T& m01(void);
			inline const T& m01(void) const;

			inline T& m02(void);
			inline const T& m02(void) const;

			inline T& m03(void);
			inline const T& m03(void) const;

			inline T& m10(void);
			inline const T& m10(void) const;

			inline T& m11(void);
			inline const T& m11(void) const;

			inline T& m12(void);
			inline const T& m12(void) const;

			inline T& m13(void);
			inline const T& m13(void) const;

			inline T& m20(void);
			inline const T& m20(void) const;

			inline T& m21(void);
			inline const T& m21(void) const;

			inline T& m22(void);
			inline const T& m22(void) const;

			inline T& m23(void);
			inline const T& m23(void) const;

			inline T& m30(void);
			inline const T& m30(void) const;

			inline T& m31(void);
			inline const T& m31(void) const;

			inline T& m32(void);
			inline const T& m32(void) const;

			inline T& m33(void);
			inline const T& m33(void) const;

		private:

			T m_data[16];

		};

	}

}

#include "Matrix4.inl"