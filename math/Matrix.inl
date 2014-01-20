
#define __MYE_MATRIX_ACCESS(__MATRIX, __COLS, __I, __J) (*(__MATRIX + (__COLS) * (__I) + (__J)))

namespace mye
{

	namespace math
	{

		template <typename T, int ROWS, int COLS>
		T& Matrix<T, ROWS, COLS>::operator() (int i, int j)
		{
			return __MYE_MATRIX_ACCESS(m_data, COLS, i, j);
		}

		template <typename T, int ROWS, int COLS>
		const T& Matrix<T, ROWS, COLS>::operator() (int i, int j) const
		{
			return __MYE_MATRIX_ACCESS(m_data, COLS, i, j);
		}

		template <typename T, int M, int N, int K>
		Matrix<T, M, N> operator* (const Matrix<T, M, K> &a,
			const Matrix<T, K, N> &b)
		{

			T *curr = m_data;

			for (int i = 0; i < M; i++)
			{
				for (int j = 0; j < N; j++)
				{

					*curr = T(0);

					for (int k = 0; k < K; k++)
					{
						curr += __MYE_MATRIX_ACCESS(a.m_data, COLS, i, k) *
							__MYE_MATRIX_ACCESS(b.m_data, COLS, k, j);
					}

				}
			}

		}

	}

}