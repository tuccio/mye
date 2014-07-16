namespace mye
{

	namespace math
	{

		template <typename T>
		RayTempl<T>::RayTempl(void)
		{

		}

		template <typename T>
		RayTempl<T>::RayTempl(const Matrix<T, 3, 1> &origin, const Matrix<T, 3, 1> &direction) :
			m_origin(origin),
			m_direction(direction)
		{

		}

		template <typename T>
		RayTempl<T>::~RayTempl(void)
		{

		}

		template <typename T>
		Matrix<T, 3, 1>& RayTempl<T>::Origin(void)
		{
			return m_origin;
		}

		template <typename T>
		const Matrix<T, 3, 1>& RayTempl<T>::Origin(void) const
		{
			return m_origin;
		}

		template <typename T>
		const Matrix<T, 3, 1>& RayTempl<T>::GetOrigin(void) const
		{
			return m_origin;
		}

		template <typename T>
		void RayTempl<T>::SetOrigin(const Matrix<T, 3, 1> &origin)
		{
			m_origin = origin;
		}

		template <typename T>
		Matrix<T, 3, 1>& RayTempl<T>::Direction(void)
		{
			return m_direction;
		}

		template <typename T>
		const Matrix<T, 3, 1>& RayTempl<T>::Direction(void) const
		{
			return m_direction;
		}

		template <typename T>
		const Matrix<T, 3, 1>& RayTempl<T>::GetDirection(void) const
		{
			return m_direction;
		}

		template <typename T>
		void RayTempl<T>::SetDirection(const Matrix<T, 3, 1> &direction)
		{
			m_direction = direction;
		}

		template <typename T>
		Matrix<T, 3, 1> RayTempl<T>::Evaluate(T t) const
		{
			return m_origin + t * m_direction;
		}

	}

}