namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 3, 1>& Rayt<T>::Origin(void)
		{
			return m_origin;
		}

		template <typename T>
		const Matrix<T, 3, 1>& Rayt<T>::Origin(void) const
		{
			return m_origin;
		}

		template <typename T>
		const Matrix<T, 3, 1>& Rayt<T>::GetOrigin(void) const
		{
			return m_origin;
		}

		template <typename T>
		void Rayt<T>::SetOrigin(const Matrix<T, 3, 1> &origin)
		{
			m_origin = origin;
		}

		template <typename T>
		Matrix<T, 3, 1>& Rayt<T>::Direction(void)
		{
			return m_direction;
		}

		template <typename T>
		const Matrix<T, 3, 1>& Rayt<T>::Direction(void) const
		{
			return m_direction;
		}

		template <typename T>
		const Matrix<T, 3, 1>& Rayt<T>::GetDirection(void) const
		{
			return m_direction;
		}

		template <typename T>
		void Rayt<T>::SetDirection(const Matrix<T, 3, 1> &direction)
		{
			m_direction = direction;
		}

		template <typename T>
		Matrix<T, 3, 1> Rayt<T>::At(T t) const
		{
			return m_origin + t * m_direction;
		}

	}

}