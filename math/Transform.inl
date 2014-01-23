namespace mye
{

	namespace math
	{

		template <typename T>
		Transform<T> Transform<T>::Identity(void)
		{
			return Transform<T>(
				Matrix<T, 3, 1>(0, 0, 0),
				Quaternion<T>(1, 0, 0, 0),
				Matrix<T, 3, 1>(1, 1, 1)
				);
		}

		template <typename T>
		Transform<T>::Transform(void)
		{
		}

		template <typename T>
		Transform<T>::Transform(const Matrix<T, 3, 1> &position,
			const Quaternion<T> &orientation,
			const Matrix<T, 3, 1> &scale) :
		m_position(position),
		m_orientation(orientation),
		m_scale(scale)
		{

		}

		template <typename T>
		Transform<T>::~Transform(void)
		{
		}

		
		template <typename T>
		const Quaternion<T>& Transform<T>::GetOrientation(void) const
		{
			return m_orientation;
		}

		template <typename T>
		void Transform<T>::SetOrientation(const Quaternion<T> &q)
		{
			m_orientation = q;
		}

		template <typename T>
		const Matrix<T, 3, 1>& Transform<T>::GetPosition(void) const
		{
			return m_position;
		}

		template <typename T>
		void Transform<T>::SetPosition(const Matrix<T, 3, 1> &position)
		{
			m_position = position;
		}

		template <typename T>
		const Matrix<T, 3, 1>& Transform<T>::GetScale(void) const
		{
			return m_scale;
		}

		template <typename T>
		void Transform<T>::SetScale(const Matrix<T, 3, 1> &scale)
		{
			m_scale = scale;
		}
		
		template <typename T>
		Matrix<T, 4, 4> Transform<T>::GetMatrix(void) const
		{
			return RotationMatrix4(m_orientation) *
			ScaleMatrix4(m_scale) *
			TranslationMatrix4(m_position);
		}

		template <typename T>
		Transform<T> Transform<T>::Combine(const Transform<T> &transform)
		{
			return Transform<T>(
				m_position + transform.m_position,
				transform.m_orientation * m_orientation,
				m_scale * transform.m_scale
				);
		}

	}

}