namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 3, 1>& Triangle<T>::v0(void)
		{
			return m_v[0];
		}

		template <typename T>
		const Matrix<T, 3, 1>& Triangle<T>::v0(void) const
		{
			return m_v[0];
		}

		template <typename T>
		Matrix<T, 3, 1>& Triangle<T>::v1(void)
		{
			return m_v[1];
		}

		template <typename T>
		const Matrix<T, 3, 1>& Triangle<T>::v1(void) const
		{
			return m_v[1];
		}

		template <typename T>
		Matrix<T, 3, 1>& Triangle<T>::v2(void)
		{
			return m_v[2];
		}

		template <typename T>
		const Matrix<T, 3, 1>& Triangle<T>::v2(void) const
		{
			return m_v[2];
		}

		template <typename T>
		bool Triangle<T>::operator== (const Triangle<T> &t)
		{
			return m_v[0] == t.m_v[0] &&
				m_v[1] == t.m_v[1] &&
				m_v[2] == t.m_v[2];
		}

	}

}