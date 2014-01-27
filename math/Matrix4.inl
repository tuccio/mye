#define _00(__m) *((__m))
#define _01(__m) *((__m) + 1)
#define _02(__m) *((__m) + 2)
#define _03(__m) *((__m) + 3)
#define _10(__m) *((__m) + 4)
#define _11(__m) *((__m) + 5)
#define _12(__m) *((__m) + 6)
#define _13(__m) *((__m) + 7)
#define _20(__m) *((__m) + 8)
#define _21(__m) *((__m) + 9)
#define _22(__m) *((__m) + 10)
#define _23(__m) *((__m) + 11)
#define _30(__m) *((__m) + 12)
#define _31(__m) *((__m) + 13)
#define _32(__m) *((__m) + 14)
#define _33(__m) *((__m) + 15)

namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 4, 4>::Matrix(void)
		{
		}

		template <typename T>
		Matrix<T, 4, 4>::Matrix(T d)
		{
			_00(m_data) = d;
			_01(m_data) = T(0);
			_02(m_data) = T(0);
			_03(m_data) = T(0);
			_10(m_data) = T(0);
			_11(m_data) = d;
			_12(m_data) = T(0);
			_13(m_data) = T(0);
			_20(m_data) = T(0);
			_21(m_data) = T(0);
			_22(m_data) = d;
			_23(m_data) = T(0);
			_30(m_data) = T(0);
			_31(m_data) = T(0);
			_32(m_data) = T(0);
			_33(m_data) = d;
		}

		template <typename T>
		Matrix<T, 4, 4>::Matrix(const Matrix<T, 3, 3> &m)
		{
			_00(m_data) = m(0, 0);
			_01(m_data) = m(0, 1);
			_02(m_data) = m(0, 2);
			_03(m_data) = T(0);
			_10(m_data) = m(1, 0);
			_11(m_data) = m(1, 1);
			_12(m_data) = m(1, 2);
			_13(m_data) = T(0);
			_20(m_data) = m(2, 0);
			_21(m_data) = m(2, 1);
			_22(m_data) = m(2, 2);
			_23(m_data) = T(0);
			_30(m_data) = T(0);
			_31(m_data) = T(0);
			_32(m_data) = T(0);
			_33(m_data) = T(1);
		}

		template <typename T>
		void Matrix<T, 4, 4>::Fill(T x)
		{
			_00(m_data) = x;
			_01(m_data) = x;
			_02(m_data) = x;
			_03(m_data) = x;
			_10(m_data) = x;
			_11(m_data) = x;
			_12(m_data) = x;
			_13(m_data) = x;
			_20(m_data) = x;
			_21(m_data) = x;
			_22(m_data) = x;
			_23(m_data) = x;
			_30(m_data) = x;
			_31(m_data) = x;
			_32(m_data) = x;
			_33(m_data) = x;
		}

		template <typename T>
		Matrix<T, 4, 1> Matrix<T, 4, 4>::GetRow(int i) const
		{
			assert(i >= 0 && i < 4);
			return Matrix<T, 4, 1>(
				__MYE_MATRIX_ACCESS(m_data, 4, i, 0),
				__MYE_MATRIX_ACCESS(m_data, 4, i, 1),
				__MYE_MATRIX_ACCESS(m_data, 4, i, 2),
				__MYE_MATRIX_ACCESS(m_data, 4, i, 3)
				);
		}

		template <typename T>
		void Matrix<T, 4, 4>::SetRow(int i, const Matrix<T, 4, 1> &v)
		{
			assert(i >= 0 && i < 4);
			__MYE_MATRIX_ACCESS(m_data, 4, i, 0) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 4, i, 1) = v[1];
			__MYE_MATRIX_ACCESS(m_data, 4, i, 2) = v[2];
			__MYE_MATRIX_ACCESS(m_data, 4, i, 3) = v[3];
		}

		template <typename T>
		Matrix<T, 4, 1> Matrix<T, 4, 4>::GetColumn(int i) const
		{
			assert(i >= 0 && i < 4);
			return Matrix<T, 4, 1>(
				__MYE_MATRIX_ACCESS(m_data, 4, 0, i),
				__MYE_MATRIX_ACCESS(m_data, 4, 1, i),
				__MYE_MATRIX_ACCESS(m_data, 4, 2, i),
				__MYE_MATRIX_ACCESS(m_data, 4, 3, i)
				);
		}

		template <typename T>
		void Matrix<T, 4, 4>::SetColumn(int i, const Matrix<T, 4, 1> &v)
		{
			assert(i >= 0 && i < 4);
			__MYE_MATRIX_ACCESS(m_data, 4, 0, i) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 4, 1, i) = v[1];
			__MYE_MATRIX_ACCESS(m_data, 4, 2, i) = v[2];
			__MYE_MATRIX_ACCESS(m_data, 4, 3, i) = v[3];
		}

		template <typename T>
		T& Matrix<T, 4, 4>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 4 && j >= 0 && j < 4);
			return __MYE_MATRIX_ACCESS(m_data, 4, i, j);
		}

		template <typename T>
		const T& Matrix<T, 4, 4>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 4 && j >= 0 && j < 4);
			return __MYE_MATRIX_ACCESS(m_data, 4, i, j);
		}



		template <typename T>
		Matrix<T, 4, 4>& Matrix<T, 4, 4>::ScaleInPlace(T s)
		{
			_00(m_data) *= s;
			_01(m_data) *= s;
			_02(m_data) *= s;
			_03(m_data) *= s;
			_10(m_data) *= s;
			_11(m_data) *= s;
			_12(m_data) *= s;
			_13(m_data) *= s;
			_20(m_data) *= s;
			_21(m_data) *= s;
			_22(m_data) *= s;
			_23(m_data) *= s;
			_30(m_data) *= s;
			_31(m_data) *= s;
			_32(m_data) *= s;
			_33(m_data) *= s;
			return *this;
		}

		template <typename T>
		inline Matrix<T, 4, 4> Matrix<T, 4, 4>::operator* (
			const Matrix<T, 4, 4> &b) const
		{

			Matrix<T, 4, 4> r;

			// First row

			_00(r.m_data) = 
				_00(m_data) * _00(b.m_data) +
				_01(m_data) * _10(b.m_data) +
				_02(m_data) * _20(b.m_data) +
				_03(m_data) * _30(b.m_data);

			_01(r.m_data) = 
				_00(m_data) * _01(b.m_data) +
				_01(m_data) * _11(b.m_data) +
				_02(m_data) * _21(b.m_data) +
				_03(m_data) * _31(b.m_data);

			_02(r.m_data) = 
				_00(m_data) * _02(b.m_data) +
				_01(m_data) * _12(b.m_data) +
				_02(m_data) * _22(b.m_data) +
				_03(m_data) * _32(b.m_data);

			_03(r.m_data) = 
				_00(m_data) * _03(b.m_data) +
				_01(m_data) * _13(b.m_data) +
				_02(m_data) * _23(b.m_data) +
				_03(m_data) * _33(b.m_data);

			// Second row

			_10(r.m_data) = 
				_10(m_data) * _00(b.m_data) +
				_11(m_data) * _10(b.m_data) +
				_12(m_data) * _20(b.m_data) +
				_13(m_data) * _30(b.m_data);

			_11(r.m_data) = 
				_10(m_data) * _01(b.m_data) +
				_11(m_data) * _11(b.m_data) +
				_12(m_data) * _21(b.m_data) +
				_13(m_data) * _31(b.m_data);

			_12(r.m_data) = 
				_10(m_data) * _02(b.m_data) +
				_11(m_data) * _12(b.m_data) +
				_12(m_data) * _22(b.m_data) +
				_13(m_data) * _32(b.m_data);

			_13(r.m_data) = 
				_10(m_data) * _03(b.m_data) +
				_11(m_data) * _13(b.m_data) +
				_12(m_data) * _23(b.m_data) +
				_13(m_data) * _33(b.m_data);

			// Third row

			_20(r.m_data) = 
				_20(m_data) * _00(b.m_data) +
				_21(m_data) * _10(b.m_data) +
				_22(m_data) * _20(b.m_data) +
				_23(m_data) * _30(b.m_data);

			_21(r.m_data) = 
				_20(m_data) * _01(b.m_data) +
				_21(m_data) * _11(b.m_data) +
				_22(m_data) * _21(b.m_data) +
				_23(m_data) * _31(b.m_data);

			_22(r.m_data) = 
				_20(m_data) * _02(b.m_data) +
				_21(m_data) * _12(b.m_data) +
				_22(m_data) * _22(b.m_data) +
				_23(m_data) * _32(b.m_data);

			_23(r.m_data) = 
				_20(m_data) * _03(b.m_data) +
				_21(m_data) * _13(b.m_data) +
				_22(m_data) * _23(b.m_data) +
				_23(m_data) * _33(b.m_data);

			// Fourth row

			_30(r.m_data) = 
				_30(m_data) * _00(b.m_data) +
				_31(m_data) * _10(b.m_data) +
				_32(m_data) * _20(b.m_data) +
				_33(m_data) * _30(b.m_data);

			_31(r.m_data) = 
				_30(m_data) * _01(b.m_data) +
				_31(m_data) * _11(b.m_data) +
				_32(m_data) * _21(b.m_data) +
				_33(m_data) * _31(b.m_data);

			_32(r.m_data) = 
				_30(m_data) * _02(b.m_data) +
				_31(m_data) * _12(b.m_data) +
				_32(m_data) * _22(b.m_data) +
				_33(m_data) * _32(b.m_data);

			_33(r.m_data) = 
				_30(m_data) * _03(b.m_data) +
				_31(m_data) * _13(b.m_data) +
				_32(m_data) * _23(b.m_data) +
				_33(m_data) * _33(b.m_data);

			return r;

		}

		template <typename T>
		Matrix<T, 4, 4> Matrix<T, 4, 4>::Transpose(void) const
		{
			Matrix<T, 4, 4> r;

			_00(r.m_data) = _00(m_data);
			_01(r.m_data) = _10(m_data);
			_02(r.m_data) = _20(m_data);
			_03(r.m_data) = _30(m_data);

			_10(r.m_data) = _01(m_data);
			_11(r.m_data) = _11(m_data);
			_12(r.m_data) = _21(m_data);
			_13(r.m_data) = _31(m_data);

			_20(r.m_data) = _02(m_data);
			_21(r.m_data) = _12(m_data);
			_22(r.m_data) = _22(m_data);
			_23(r.m_data) = _32(m_data);

			_30(r.m_data) = _03(m_data);
			_31(r.m_data) = _13(m_data);
			_32(r.m_data) = _23(m_data);
			_33(r.m_data) = _33(m_data);

			return r;

		}

		template <typename T>
		Matrix<T, 4, 4> Matrix<T, 4, 4>::Inverse(void) const
		{

			Matrix<T, 4, 4> r;

			_00(r.m_data) = _12(m_data) * _23(m_data) * _31(m_data)
				- _13(m_data) * _22(m_data) * _31(m_data)
				+ _13(m_data) * _21(m_data) * _32(m_data)
				- _11(m_data) * _23(m_data) * _32(m_data)
				- _12(m_data) * _21(m_data) * _33(m_data)
				+ _11(m_data) * _22(m_data) * _33(m_data);

			_01(r.m_data) = _03(m_data) * _22(m_data) * _31(m_data)
				- _02(m_data) * _23(m_data) * _31(m_data)
				- _03(m_data) * _21(m_data) * _32(m_data)
				+ _01(m_data) * _23(m_data) * _32(m_data)
				+ _02(m_data) * _21(m_data) * _33(m_data)
				- _01(m_data) * _22(m_data) * _33(m_data);

			_02(r.m_data) = _02(m_data) * _13(m_data) * _31(m_data)
				- _03(m_data) * _12(m_data) * _31(m_data)
				+ _03(m_data) * _11(m_data) * _32(m_data)
				- _01(m_data) * _13(m_data) * _32(m_data)
				- _02(m_data) * _11(m_data) * _33(m_data)
				+ _01(m_data) * _12(m_data) * _33(m_data);

			_03(r.m_data) = _03(m_data) * _12(m_data) * _21(m_data)
				- _02(m_data) * _13(m_data) * _21(m_data)
				- _03(m_data) * _11(m_data) * _22(m_data)
				+ _01(m_data) * _13(m_data) * _22(m_data)
				+ _02(m_data) * _11(m_data) * _23(m_data)
				- _01(m_data) * _12(m_data) * _23(m_data);

			_10(r.m_data) = _13(m_data) * _22(m_data) * _30(m_data)
				- _12(m_data) * _23(m_data) * _30(m_data)
				- _13(m_data) * _20(m_data) * _32(m_data)
				+ _10(m_data) * _23(m_data) * _32(m_data)
				+ _12(m_data) * _20(m_data) * _33(m_data)
				- _10(m_data) * _22(m_data) * _33(m_data);

			_11(r.m_data) = _02(m_data) * _23(m_data) * _30(m_data)
				- _03(m_data) * _22(m_data) * _30(m_data)
				+ _03(m_data) * _20(m_data) * _32(m_data)
				- _00(m_data) * _23(m_data) * _32(m_data)
				- _02(m_data) * _20(m_data) * _33(m_data)
				+ _00(m_data) * _22(m_data) * _33(m_data);

			_12(r.m_data) = _03(m_data) * _12(m_data) * _30(m_data)
				- _02(m_data) * _13(m_data) * _30(m_data)
				- _03(m_data) * _10(m_data) * _32(m_data)
				+ _00(m_data) * _13(m_data) * _32(m_data)
				+ _02(m_data) * _10(m_data) * _33(m_data)
				- _00(m_data) * _12(m_data) * _33(m_data);

			_13(r.m_data) = _02(m_data) * _13(m_data) * _20(m_data)
				- _03(m_data) * _12(m_data) * _20(m_data)
				+ _03(m_data) * _10(m_data) * _22(m_data)
				- _00(m_data) * _13(m_data) * _22(m_data)
				- _02(m_data) * _10(m_data) * _23(m_data)
				+ _00(m_data) * _12(m_data) * _23(m_data);

			_20(r.m_data) = _11(m_data) * _23(m_data) * _30(m_data)
				- _13(m_data) * _21(m_data) * _30(m_data)
				+ _13(m_data) * _20(m_data) * _31(m_data)
				- _10(m_data) * _23(m_data) * _31(m_data)
				- _11(m_data) * _20(m_data) * _33(m_data)
				+ _10(m_data) * _21(m_data) * _33(m_data);

			_21(r.m_data) = _03(m_data) * _21(m_data) * _30(m_data)
				- _01(m_data) * _23(m_data) * _30(m_data)
				- _03(m_data) * _20(m_data) * _31(m_data)
				+ _00(m_data) * _23(m_data) * _31(m_data)
				+ _01(m_data) * _20(m_data) * _33(m_data)
				- _00(m_data) * _21(m_data) * _33(m_data);

			_22(r.m_data) = _01(m_data) * _13(m_data) * _30(m_data)
				- _03(m_data) * _11(m_data) * _30(m_data)
				+ _03(m_data) * _10(m_data) * _31(m_data)
				- _00(m_data) * _13(m_data) * _31(m_data)
				- _01(m_data) * _10(m_data) * _33(m_data)
				+ _00(m_data) * _11(m_data) * _33(m_data);

			_23(r.m_data) = _03(m_data) * _11(m_data) * _20(m_data)
				- _01(m_data) * _13(m_data) * _20(m_data)
				- _03(m_data) * _10(m_data) * _21(m_data)
				+ _00(m_data) * _13(m_data) * _21(m_data)
				+ _01(m_data) * _10(m_data) * _23(m_data)
				- _00(m_data) * _11(m_data) * _23(m_data);

			_30(r.m_data) = _12(m_data) * _21(m_data) * _30(m_data)
				- _11(m_data) * _22(m_data) * _30(m_data)
				- _12(m_data) * _20(m_data) * _31(m_data)
				+ _10(m_data) * _22(m_data) * _31(m_data)
				+ _11(m_data) * _20(m_data) * _32(m_data)
				- _10(m_data) * _21(m_data) * _32(m_data);

			_31(r.m_data) = _01(m_data) * _22(m_data) * _30(m_data)
				- _02(m_data) * _21(m_data) * _30(m_data)
				+ _02(m_data) * _20(m_data) * _31(m_data)
				- _00(m_data) * _22(m_data) * _31(m_data)
				- _01(m_data) * _20(m_data) * _32(m_data)
				+ _00(m_data) * _21(m_data) * _32(m_data);

			_32(r.m_data) = _02(m_data) * _11(m_data) * _30(m_data)
				- _01(m_data) * _12(m_data) * _30(m_data)
				- _02(m_data) * _10(m_data) * _31(m_data)
				+ _00(m_data) * _12(m_data) * _31(m_data)
				+ _01(m_data) * _10(m_data) * _32(m_data)
				- _00(m_data) * _11(m_data) * _32(m_data);

			_33(r.m_data) = _01(m_data) * _12(m_data) * _20(m_data)
				- _02(m_data) * _11(m_data) * _20(m_data)
				+ _02(m_data) * _10(m_data) * _21(m_data)
				- _00(m_data) * _12(m_data) * _21(m_data)
				- _01(m_data) * _10(m_data) * _22(m_data)
				+ _00(m_data) * _11(m_data) * _22(m_data);

			return r.ScaleInPlace(1.0f / r.Determinant());

		}

		template <typename T>
		T Matrix<T, 4, 4>::Determinant(void) const
		{

			return _03(m_data) * _12(m_data) * _21(m_data) * _30(m_data)
				- _02(m_data) * _13(m_data) * _21(m_data) * _30(m_data)
				- _03(m_data) * _11(m_data) * _22(m_data) * _30(m_data)
				+ _01(m_data) * _13(m_data) * _22(m_data) * _30(m_data)
				+ _02(m_data) * _11(m_data) * _23(m_data) * _30(m_data)
				- _01(m_data) * _12(m_data) * _23(m_data) * _30(m_data)
				- _03(m_data) * _12(m_data) * _20(m_data) * _31(m_data)
				+ _02(m_data) * _13(m_data) * _20(m_data) * _31(m_data)
				+ _03(m_data) * _10(m_data) * _22(m_data) * _31(m_data)
				- _00(m_data) * _13(m_data) * _22(m_data) * _31(m_data)
				- _02(m_data) * _10(m_data) * _23(m_data) * _31(m_data)
				+ _00(m_data) * _12(m_data) * _23(m_data) * _31(m_data)
				+ _03(m_data) * _11(m_data) * _20(m_data) * _32(m_data)
				- _01(m_data) * _13(m_data) * _20(m_data) * _32(m_data)
				- _03(m_data) * _10(m_data) * _21(m_data) * _32(m_data)
				+ _00(m_data) * _13(m_data) * _21(m_data) * _32(m_data)
				+ _01(m_data) * _10(m_data) * _23(m_data) * _32(m_data)
				- _00(m_data) * _11(m_data) * _23(m_data) * _32(m_data)
				- _02(m_data) * _11(m_data) * _20(m_data) * _33(m_data)
				+ _01(m_data) * _12(m_data) * _20(m_data) * _33(m_data)
				+ _02(m_data) * _10(m_data) * _21(m_data) * _33(m_data)
				- _00(m_data) * _12(m_data) * _21(m_data) * _33(m_data)
				- _01(m_data) * _10(m_data) * _22(m_data) * _33(m_data)
				+ _00(m_data) * _11(m_data) * _22(m_data) * _33(m_data);

		}

		template <typename T>
		T* Matrix<T, 4, 4>::Data(void)
		{
			return m_data;
		}

		template <typename T>
		const T* Matrix<T, 4, 4>::Data(void) const
		{
			return m_data;
		}

	}

}

#undef _00
#undef _01
#undef _02
#undef _03
#undef _10
#undef _11
#undef _12
#undef _13
#undef _20
#undef _21
#undef _22
#undef _23
#undef _30
#undef _31
#undef _32
#undef _33