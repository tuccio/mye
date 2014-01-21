namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 4, 1>::Matrix(void)
		{
		}

		template <typename T>
		Matrix<T, 4, 1>::Matrix(T x, T y, T z, T w)
		{
			m_data[0] = x;
			m_data[1] = y;
			m_data[2] = z;
			m_data[3] = w;
		}

		template <typename T>
		T& Matrix<T, 4, 1>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 4 && j == 0);
			return m_data[i];
		}

		template <typename T>
		const T& Matrix<T, 4, 1>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 4 && j == 0);
			return m_data[i];
		}

		template <typename T>
		T& Matrix<T, 4, 1>::operator[] (int i)
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}

		template <typename T>
		const T& Matrix<T, 4, 1>::operator[] (int i) const
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}

		template <typename T>
		T Matrix<T, 4, 1>::Dot(const Matrix<T, 4, 1> &v) const
		{
			return m_data[0] * v.m_data[0] +
				m_data[1] * v.m_data[1] +
				m_data[2] * v.m_data[2] +
				m_data[3] * v.m_data[3];
		}

		template <typename T>
		Matrix<T, 4, 1> Matrix<T, 4, 1>::operator+ (const Matrix<T, 4, 1> &b)
		{
			Matrix<T, 4, 1>(
				m_data[0] + b.m_data[0] +
				m_data[1] + b.m_data[1],
				m_data[2] + b.m_data[2],
				m_data[3] + b.m_data[3]);
		}

		template <typename T>
		Matrix<T, 4, 1> Matrix<T, 4, 1>::operator- (const Matrix<T, 4, 1> &b)
		{
			Matrix<T, 4, 1>(
				m_data[0] - b.m_data[0] +
				m_data[1] - b.m_data[1],
				m_data[2] - b.m_data[2],
				m_data[3] - b.m_data[3]);
		}

		template <typename T>
		Matrix<T, 4, 1> Matrix<T, 4, 1>::Normalize(void) const
		{
			T norm = Length();
			return Matrix<T, 4, 1>(
				m_data[0] / norm,
				m_data[1] / norm,
				m_data[2] / norm,
				m_data[3] / norm
				);
		}

		template <typename T>
		T Matrix<T, 4, 1>::Length(void) const
		{
			return Sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1]);
		}

		template <typename T>
		T& Matrix<T, 4, 1>::x(void)
		{
			return m_data[0];
		}

		template <typename T>
		const T& Matrix<T, 4, 1>::x(void) const
		{
			return m_data[0];
		}

		template <typename T>
		T& Matrix<T, 4, 1>::y(void)
		{
			return m_data[1];
		}

		template <typename T>
		const T& Matrix<T, 4, 1>::y(void) const
		{
			return m_data[1];
		}

		template <typename T>
		T& Matrix<T, 4, 1>::z(void)
		{
			return m_data[2];
		}

		template <typename T>
		const T& Matrix<T, 4, 1>::z(void) const
		{
			return m_data[2];
		}

		template <typename T>
		T& Matrix<T, 4, 1>::w(void)
		{
			return m_data[3];
		}

		template <typename T>
		const T& Matrix<T, 4, 1>::w(void) const
		{
			return m_data[3];
		}

		template <typename T>
		T* Matrix<T, 4, 1>::Data(void)
		{
			return m_data;
		}

		template <typename T>
		const T* Matrix<T, 4, 1>::Data(void) const
		{
			return m_data;
		}

	}

}