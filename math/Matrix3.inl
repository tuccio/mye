namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 3, 3>::Matrix(void)
		{
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
			const Matrix<T, 3, 3> &b)
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