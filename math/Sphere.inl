namespace mye
{

	namespace math
	{

		template <typename T>
		const Matrix<T, 3, 1> Sphere<T>::GetCenter(void) const
		{
			return m_center;
		}

		template <typename T>
		void Sphere<T>::SetCenter(const Matrix<T, 3, 1> &center)
		{
			m_center = center;
		}

		template <typename T>
		T Sphere<T>::GetRadius(void) const
		{
			return m_radius;
		}

		template <typename T>
		void Sphere<T>::SetRadius(T radius)
		{
			m_radius = radius;
		}

	}

}