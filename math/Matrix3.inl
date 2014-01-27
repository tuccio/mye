#define _00(__m) *((__m))
#define _01(__m) *((__m) + 1)
#define _02(__m) *((__m) + 2)
#define _10(__m) *((__m) + 3)
#define _11(__m) *((__m) + 4)
#define _12(__m) *((__m) + 5)
#define _20(__m) *((__m) + 6)
#define _21(__m) *((__m) + 7)
#define _22(__m) *((__m) + 8)

namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 3, 3>::Matrix(void)
		{
		}

		template <typename T>
		Matrix<T, 3, 3>::Matrix(T d)
		{
			_00(m_data) = d;
			_01(m_data) = T(0);
			_02(m_data) = T(0);
			_10(m_data) = T(0);
			_11(m_data) = d;
			_12(m_data) = T(0);
			_20(m_data) = T(0);
			_21(m_data) = T(0);
			_22(m_data) = d;
		}

		template <typename T>
		void Matrix<T, 3, 3>::Fill(T x)
		{
			_00(m_data) = x;
			_01(m_data) = x;
			_02(m_data) = x;
			_10(m_data) = x;
			_11(m_data) = x;
			_12(m_data) = x;
			_20(m_data) = x;
			_21(m_data) = x;
			_22(m_data) = x;
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 3>::GetRow(int i) const
		{
			assert(i >= 0 && i < 3);
			return Matrix<T, 3, 1>(
				__MYE_MATRIX_ACCESS(m_data, 3, i, 0),
				__MYE_MATRIX_ACCESS(m_data, 3, i, 1),
				__MYE_MATRIX_ACCESS(m_data, 3, i, 2)
				);
		}

		template <typename T>
		void Matrix<T, 3, 3>::SetRow(int i, const Matrix<T, 3, 1> &v)
		{
			assert(i >= 0 && i < 3);
			__MYE_MATRIX_ACCESS(m_data, 3, i, 0) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 3, i, 1) = v[1];
			__MYE_MATRIX_ACCESS(m_data, 3, i, 2) = v[2];
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 3>::GetColumn(int j) const
		{
			assert(j >= 0 && j < 3);
			return Matrix<T, 3, 1>(
				__MYE_MATRIX_ACCESS(m_data, 3, 0, j),
				__MYE_MATRIX_ACCESS(m_data, 3, 1, j),
				__MYE_MATRIX_ACCESS(m_data, 3, 2, j)
				);
		}

		template <typename T>
		void Matrix<T, 3, 3>::SetColumn(int j, const Matrix<T, 3, 1> &v)
		{
			assert(j >= 0 && j < 3);
			__MYE_MATRIX_ACCESS(m_data, 3, 0, j) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 3, 1, j) = v[1];
			__MYE_MATRIX_ACCESS(m_data, 3, 2, j) = v[2];
		}

		template <typename T>
		T& Matrix<T, 3, 3>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 3 && j >= 0 && j < 3);
			return __MYE_MATRIX_ACCESS(m_data, 3, i, j);
		}

		template <typename T>
		const T& Matrix<T, 3, 3>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 3 && j >= 0 && j < 3);
			return __MYE_MATRIX_ACCESS(m_data, 3, i, j);
		}

		template <typename T>
		Matrix<T, 3, 3> Matrix<T, 3, 3>::operator* (
			const Matrix<T, 3, 3> &b) const
		{

			Matrix<T, 3, 3> r;

			// First row

			_00(r.m_data) = 
				_00(m_data) * _00(b.m_data) +
				_01(m_data) * _10(b.m_data) +
				_02(m_data) * _20(b.m_data);

			_01(r.m_data) = 
				_00(m_data) * _01(b.m_data) +
				_01(m_data) * _11(b.m_data) +
				_02(m_data) * _21(b.m_data);

			_02(r.m_data) = 
				_00(m_data) * _02(b.m_data) +
				_01(m_data) * _12(b.m_data) +
				_02(m_data) * _22(b.m_data);

			// Second row

			_10(r.m_data) = 
				_10(m_data) * _00(b.m_data) +
				_11(m_data) * _10(b.m_data) +
				_12(m_data) * _20(b.m_data);

			_11(r.m_data) = 
				_10(m_data) * _01(b.m_data) +
				_11(m_data) * _11(b.m_data) +
				_12(m_data) * _21(b.m_data);

			_12(r.m_data) = 
				_10(m_data) * _02(b.m_data) +
				_11(m_data) * _12(b.m_data) +
				_12(m_data) * _22(b.m_data);

			// Third row

			_20(r.m_data) = 
				_20(m_data) * _00(b.m_data) +
				_21(m_data) * _10(b.m_data) +
				_22(m_data) * _20(b.m_data);

			_21(r.m_data) = 
				_20(m_data) * _01(b.m_data) +
				_21(m_data) * _11(b.m_data) +
				_22(m_data) * _21(b.m_data);

			_22(r.m_data) = 
				_20(m_data) * _02(b.m_data) +
				_21(m_data) * _12(b.m_data) +
				_22(m_data) * _22(b.m_data);

			return r;

		}

		template <typename T>
		Matrix<T, 3, 3>& Matrix<T, 3, 3>::ScaleInPlace(T s)
		{
			_00(m_data) *= s;
			_01(m_data) *= s;
			_02(m_data) *= s;
			_10(m_data) *= s;
			_11(m_data) *= s;
			_12(m_data) *= s;
			_20(m_data) *= s;
			_21(m_data) *= s;
			_22(m_data) *= s;
			_30(m_data) *= s;
			_31(m_data) *= s;
			_32(m_data) *= s;
			return *this;
		}

		template <typename T>
		Matrix<T, 3, 3> Matrix<T, 3, 3>::Transpose(void) const
		{
			Matrix<T, 3, 3> r;

			_00(r.m_data) = _00(m_data);
			_01(r.m_data) = _10(m_data);
			_02(r.m_data) = _20(m_data);

			_10(r.m_data) = _01(m_data);
			_11(r.m_data) = _11(m_data);
			_12(r.m_data) = _21(m_data);

			_20(r.m_data) = _02(m_data);
			_21(r.m_data) = _12(m_data);
			_22(r.m_data) = _22(m_data);

			return r;

		}

		template <typename T>
		T Matrix<T, 3, 3>::Determinant(void) const
		{

			return _00(m_data) * _11(m_data) * _22(m_data)
				+ _01(m_data) * _12(m_data) * _20(m_data)
				+ _02(m_data) * _10(m_data) * _21(m_data)
				- _00(m_data) * _12(m_data) * _21(m_data)
				- _01(m_data) * _10(m_data) * _22(m_data)
				- _02(m_data) * _11(m_data) * _20(m_data);

		}

		template <typename T>
		Matrix<T, 3, 3> Matrix<T, 3, 3>::Inverse(void) const
		{

			Matrix<T, 3, 3> r;

			_00(r.m_data) =
				_11(m_data) * _22(m_data)
				- _12(m_data) * _21(m_data);

			_01(r.m_data) =
				_02(m_data) * _21(m_data)
				- _01(m_data) * _22(m_data);

			_02(r.m_data) =
				_01(m_data) * _12(m_data)
				- _02(m_data) * _11(m_data);

			_10(r.m_data) =
				_12(m_data) * _20(m_data)
				- _10(m_data) * _22(m_data);

			_11(r.m_data) =
				_00(m_data) * _22(m_data)
				- _02(m_data) * _20(m_data);

			_12(r.m_data) =
				_02(m_data) * _10(m_data)
				- _00(m_data) * _12(m_data);

			_20(r.m_data) =
				_10(m_data) * _21(m_data)
				- _11(m_data) * _20(m_data);

			_21(r.m_data) =
				_01(m_data) * _20(m_data)
				- _00(m_data) * _21(m_data);

			_22(r.m_data) =
				_00(m_data) * _11(m_data)
				- _01(m_data) * _10(m_data);

			return r.ScaleInPlace(1.0f / r.Determinant());

		}

		template <typename T>
		T* Matrix<T, 3, 3>::Data(void)
		{
			return m_data;
		}

		template <typename T>
		const T* Matrix<T, 3, 3>::Data(void) const
		{
			return m_data;
		}

	}

}

#undef _00
#undef _01
#undef _02
#undef _10
#undef _11
#undef _12
#undef _20
#undef _21
#undef _22