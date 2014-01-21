namespace mye
{

	namespace math
	{

		/* Matrix class implementation */

		template <typename T, int ROWS, int COLS>
		Matrix<T, ROWS, COLS>::Matrix(void)
		{
		}

		template <typename T, int ROWS, int COLS>
		void Matrix<T, ROWS, COLS>::Fill(T x)
		{
			T *curr = m_data;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					*curr = x;
					curr++;
				}
			}
		}

		template <typename T, int ROWS, int COLS>
		T& Matrix<T, ROWS, COLS>::operator() (int i, int j)
		{
			assert(i >= 0 && i < ROWS && j >= 0 && j < COLS);
			return __MYE_MATRIX_ACCESS(m_data, COLS, i, j);
		}

		template <typename T, int ROWS, int COLS>
		const T& Matrix<T, ROWS, COLS>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < ROWS && j >= 0 && j < COLS);
			return __MYE_MATRIX_ACCESS(m_data, COLS, i, j);
		}

		template <typename T, int ROWS, int COLS>
		template <int N>
		Matrix<T, ROWS, N> Matrix<T, ROWS, COLS>::operator* (const Matrix<T, COLS, N> &b) const
		{

			Matrix<T, ROWS, N> r;
			T *curr = r.m_data;

			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < N; j++)
				{

					*curr = T(0);

					for (int k = 0; k < COLS; k++)
					{
						*curr += __MYE_MATRIX_ACCESS(m_data, COLS, i, k) *
							__MYE_MATRIX_ACCESS(b.m_data, N, k, j);
					}

					curr++;

				}
			}

			return r;

		}

		template <typename T, int M, int K, int N>
		Matrix<T, M, N> operator* (const Matrix<T, M, K> &a,
			const Matrix<T, K, N> &b)
		{
			
			Matrix<T, M, N> r;

			for (int i = 0; i < M; i++)
			{

				for (int j = 0; j < N; j++)
				{

					r(i, j) = T(0);

					for (int k = 0; k < K; k++)
					{

						r(i, j) += a(i, k) * b(k, j);

					}

				}

			}

			return r;

		}

		template <typename T, int ROWS, int COLS>
		Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator+ (const Matrix<T, ROWS, COLS> &b) const
		{
			Matrix<T, ROWS, COLS> r;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					__MYE_MATRIX_ACCESS(r.m_data, COLS, i, j) =
						__MYE_MATRIX_ACCESS(m_data, COLS, i, j) +
						__MYE_MATRIX_ACCESS(b.m_data, COLS, i, j);
				}
			}
			return r;
		}

		template <typename T, int ROWS, int COLS>
		Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator- (const Matrix<T, ROWS, COLS> &b) const
		{
			Matrix<T, ROWS, COLS> r;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					__MYE_MATRIX_ACCESS(r.m_data, COLS, i, j) =
						__MYE_MATRIX_ACCESS(m_data, COLS, i, j) -
						__MYE_MATRIX_ACCESS(b.m_data, COLS, i, j);
				}
			}
			return r;
		}

		template <typename T, int ROWS, int COLS>
		T* Matrix<T, ROWS, COLS>::Data(void)
		{
			return m_data;
		}

		template <typename T, int ROWS, int COLS>
		const T* Matrix<T, ROWS, COLS>::Data(void) const
		{
			return m_data;
		}

	}

}