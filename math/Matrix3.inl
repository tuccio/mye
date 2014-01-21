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
			__MYE_MATRIX_ACCESS(m_data, 3, 0, 0) = d;
			__MYE_MATRIX_ACCESS(m_data, 3, 0, 1) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 3, 0, 2) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 3, 1, 0) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 3, 1, 1) = d;
			__MYE_MATRIX_ACCESS(m_data, 3, 1, 2) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 3, 2, 0) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 3, 2, 1) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 3, 2, 2) = d;
		}

		template <typename T>
		void Matrix<T, 3, 3>::Fill(T x)
		{
			__MYE_MATRIX_ACCESS(m_data, 3, 0, 0) = x;
			__MYE_MATRIX_ACCESS(m_data, 3, 0, 1) = x;
			__MYE_MATRIX_ACCESS(m_data, 3, 0, 2) = x;
			__MYE_MATRIX_ACCESS(m_data, 3, 1, 0) = x;
			__MYE_MATRIX_ACCESS(m_data, 3, 1, 1) = x;
			__MYE_MATRIX_ACCESS(m_data, 3, 1, 2) = x;
			__MYE_MATRIX_ACCESS(m_data, 3, 2, 0) = x;
			__MYE_MATRIX_ACCESS(m_data, 3, 2, 1) = x;
			__MYE_MATRIX_ACCESS(m_data, 3, 2, 2) = x;
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
		inline Matrix<T, 3, 3> Matrix<T, 3, 3>::operator* (
			const Matrix<T, 3, 3> &b) const
		{

			Matrix<T, 3, 3> r;

			// First row

			__MYE_MATRIX_ACCESS(r.m_data, 3, 0, 0) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 0) +
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 0) +
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 0);

			__MYE_MATRIX_ACCESS(r.m_data, 3, 0, 1) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 1) +
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 1) +
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 1);

			__MYE_MATRIX_ACCESS(r.m_data, 3, 0, 2) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 2) +
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 2) +
				__MYE_MATRIX_ACCESS(m_data, 3, 0, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 2);

			// Second row

			__MYE_MATRIX_ACCESS(r.m_data, 3, 1, 0) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 0) +
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 0) +
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 0);

			__MYE_MATRIX_ACCESS(r.m_data, 3, 1, 1) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 1) +
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 1) +
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 1);

			__MYE_MATRIX_ACCESS(r.m_data, 3, 1, 2) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 2) +
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 2) +
				__MYE_MATRIX_ACCESS(m_data, 3, 1, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 2);

			// Third row

			__MYE_MATRIX_ACCESS(r.m_data, 3, 2, 0) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 0) +
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 0) +
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 0);

			__MYE_MATRIX_ACCESS(r.m_data, 3, 2, 1) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 1) +
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 1) +
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 1);

			__MYE_MATRIX_ACCESS(r.m_data, 3, 2, 2) = 
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 0) * __MYE_MATRIX_ACCESS(b.m_data, 3, 0, 2) +
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 1) * __MYE_MATRIX_ACCESS(b.m_data, 3, 1, 2) +
				__MYE_MATRIX_ACCESS(m_data, 3, 2, 2) * __MYE_MATRIX_ACCESS(b.m_data, 3, 2, 2);

			return r;

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