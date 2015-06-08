#define _00(__m) *((__m))
#define _01(__m) *((__m) + 1)
#define _10(__m) *((__m) + 2)
#define _11(__m) *((__m) + 3)

namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2>::Matrix(void)
		{
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2>::Matrix(T d)
		{
			_00(m_data) = d;
			_01(m_data) = T(0);
			_10(m_data) = T(0);
			_11(m_data) = d;
		}

		template <typename T>
		__MYE_MATH_INLINE void Matrix<T, 2, 2>::Fill(T x)
		{
			_00(m_data) = x;
			_01(m_data) = x;
			_10(m_data) = x;
			_11(m_data) = x;
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 1> Matrix<T, 2, 2>::GetRow(int i) const
		{
			assert(i >= 0 && i < 2);
			return Matrix<T, 2, 1>(__MYE_MATRIX_ACCESS(m_data, 2, i, 0),
			                       __MYE_MATRIX_ACCESS(m_data, 2, i, 1));
		}

		template <typename T>
		__MYE_MATH_INLINE void Matrix<T, 2, 2>::SetRow(int i, const Matrix<T, 2, 1> & v)
		{
			assert(i >= 0 && i < 2);
			__MYE_MATRIX_ACCESS(m_data, 2, i, 0) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 2, i, 1) = v[1];
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 1> Matrix<T, 2, 2>::GetColumn(int j) const
		{
			assert(j >= 0 && j < 2);
			return Matrix<T, 2, 1>(__MYE_MATRIX_ACCESS(m_data, 2, 0, j),
			                       __MYE_MATRIX_ACCESS(m_data, 2, 1, j));
		}

		template <typename T>
		__MYE_MATH_INLINE void Matrix<T, 2, 2>::SetColumn(int j, const Matrix<T, 2, 1> &v)
		{
			assert(j >= 0 && j < 2);
			__MYE_MATRIX_ACCESS(m_data, 3, 0, j) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 3, 1, j) = v[1];
			__MYE_MATRIX_ACCESS(m_data, 3, 2, j) = v[2];
		}

		template <typename T>
		__MYE_MATH_INLINE T & Matrix<T, 2, 2>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 2 && j >= 0 && j < 2);
			return __MYE_MATRIX_ACCESS(m_data, 2, i, j);
		}

		template <typename T>
		__MYE_MATH_INLINE const T& Matrix<T, 2, 2>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 2 && j >= 0 && j < 2);
			return __MYE_MATRIX_ACCESS(m_data, 2, i, j);
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> Matrix<T, 2, 2>::operator- (void) const
		{

			Matrix<T, 2, 2> r;

			_00(r.m_data) = -_00(m_data);
			_01(r.m_data) = -_01(m_data);
			_10(r.m_data) = -_10(m_data);
			_11(r.m_data) = -_11(m_data);

			return r;

		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> & Matrix<T, 2, 2>::operator= (const Matrix<T, 2, 2> & a)
		{

			_00(m_data) = _00(a.m_data);
			_01(m_data) = _01(a.m_data);
			_10(m_data) = _10(a.m_data);
			_11(m_data) = _11(a.m_data);

			return *this;

		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> Matrix<T, 2, 2>::operator+ (const Matrix<T, 2, 2> & a) const
		{

			Matrix<T, 2, 2> r;

			_00(r.m_data) = _00(m_data) + _00(a.m_data);
			_01(r.m_data) = _01(m_data) + _01(a.m_data);
			_10(r.m_data) = _10(m_data) + _10(a.m_data);
			_11(r.m_data) = _11(m_data) + _11(a.m_data);

			return r;

		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> Matrix<T, 2, 2>::operator- (const Matrix<T, 2, 2> & a) const
		{

			Matrix<T, 2, 2> r;

			_00(r.m_data) = _00(m_data) - _00(a.m_data);
			_01(r.m_data) = _01(m_data) - _01(a.m_data);
			_10(r.m_data) = _10(m_data) - _10(a.m_data);
			_11(r.m_data) = _11(m_data) - _11(a.m_data);

			return r;

		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> Matrix<T, 2, 2>::operator* (const Matrix<T, 2, 2> & b) const
		{

			Matrix<T, 2, 2> r;

			// First row

			_00(r.m_data) =
				_00(m_data) * _00(b.m_data) +
				_01(m_data) * _10(b.m_data);

			_01(r.m_data) =
				_00(m_data) * _01(b.m_data) +
				_01(m_data) * _11(b.m_data);

			// Second row

			_10(r.m_data) =
				_10(m_data) * _00(b.m_data) +
				_11(m_data) * _10(b.m_data);

			_11(r.m_data) =
				_10(m_data) * _01(b.m_data) +
				_11(m_data) * _11(b.m_data);

			return r;

		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> Matrix<T, 2, 2>::operator* (T s) const
		{

			Matrix<T, 2, 2> r;

			_00(r.m_data) = _00(m_data) * s;
			_01(r.m_data) = _01(m_data) * s;
			_10(r.m_data) = _10(m_data) * s;
			_11(r.m_data) = _11(m_data) * s;

			return r;

		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> Matrix<T, 2, 2>::operator/ (T s) const
		{
			return *this * (T(1) / s);
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> Matrix<T, 2, 2>::Transpose(void) const
		{

			Matrix<T, 2, 2> r;

			_00(r.m_data) = _00(m_data);
			_01(r.m_data) = _10(m_data);

			_10(r.m_data) = _01(m_data);
			_11(r.m_data) = _11(m_data);

			return r;

		}

		template <typename T>
		__MYE_MATH_INLINE T Matrix<T, 2, 2>::Determinant(void) const
		{
			return _00(m_data) * _11(m_data) - _01(m_data) * _10(m_data);
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> Matrix<T, 2, 2>::Inverse(void) const
		{

			Matrix<T, 2, 2> r;

			_11(r.m_data) =   _00(m_data);
			_01(r.m_data) = - _01(m_data);
			_10(r.m_data) = - _10(m_data);
			_00(r.m_data) =   _11(m_data);

			float determinant = Determinant();
			assert(determinant != T(0) && "Cannot invert a singular matrix");

			return r / determinant;

		}

		template <typename T>
		__MYE_MATH_INLINE T * Matrix<T, 2, 2>::Data(void)
		{
			return m_data;
		}

		template <typename T>
		__MYE_MATH_INLINE const T * Matrix<T, 2, 2>::Data(void) const
		{
			return m_data;
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 2, 2> operator* (T s, const Matrix<T, 2, 2> & m)
		{
			return m * s;
		}

	}

}

#undef _00
#undef _01
#undef _10
#undef _11