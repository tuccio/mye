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
			__MYE_MATRIX_ACCESS(m_data, 4, 0, 0) = d;
			__MYE_MATRIX_ACCESS(m_data, 4, 0, 1) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 0, 2) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 0, 3) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 1, 0) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 1, 1) = d;
			__MYE_MATRIX_ACCESS(m_data, 4, 1, 2) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 1, 3) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 2, 0) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 2, 1) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 2, 2) = d;
			__MYE_MATRIX_ACCESS(m_data, 4, 2, 3) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 3, 0) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 3, 1) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 3, 2) = T(0);
			__MYE_MATRIX_ACCESS(m_data, 4, 3, 3) = d;
		}

		template <typename T>
		void Matrix<T, 4, 4>::Fill(T x)
		{
			__MYE_MATRIX_ACCESS(m_data, 4, 0, 0) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 0, 1) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 0, 2) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 0, 3) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 1, 0) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 1, 1) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 1, 2) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 1, 3) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 2, 0) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 2, 1) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 2, 2) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 2, 3) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 3, 0) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 3, 1) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 3, 2) = x;
			__MYE_MATRIX_ACCESS(m_data, 4, 3, 3) = x;
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
		inline Matrix<T, 4, 4> Matrix<T, 4, 4>::operator* (
			const Matrix<T, 4, 4> &b) const
		{

			Matrix<T, 4, 4> r;

			// First row

			__MYE_MATRIX_ACCESS(r.m_data, 4, 0, 0) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 0);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 0, 1) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 1);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 0, 2) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 2);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 0, 3) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 0, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 3);

			// Second row

			__MYE_MATRIX_ACCESS(r.m_data, 4, 1, 0) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 0);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 1, 1) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 1);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 1, 2) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 2);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 1, 3) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 1, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 3);

			// Third row

			__MYE_MATRIX_ACCESS(r.m_data, 4, 2, 0) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 0);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 2, 1) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 1);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 2, 2) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 2);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 2, 3) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 2, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 3);

			// Fourth row

			__MYE_MATRIX_ACCESS(r.m_data, 4, 3, 0) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 0) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 0);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 3, 1) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 1) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 1);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 3, 2) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 2) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 2);

			__MYE_MATRIX_ACCESS(r.m_data, 4, 3, 3) = 
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 0) * __MYE_MATRIX_ACCESS(b.m_data, 4, 0, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 1) * __MYE_MATRIX_ACCESS(b.m_data, 4, 1, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 2) * __MYE_MATRIX_ACCESS(b.m_data, 4, 2, 3) +
				__MYE_MATRIX_ACCESS(m_data, 4, 3, 3) * __MYE_MATRIX_ACCESS(b.m_data, 4, 3, 3);

			return r;

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