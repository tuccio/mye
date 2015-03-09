namespace mye
{

	namespace math
	{

		template <typename T>
		TransformTempl<T> TransformTempl<T>::Identity(void)
		{
			return TransformTempl<T>(
				Matrix<T, 3, 1>(0, 0, 0),
				QuaternionTempl<T>(1, 0, 0, 0),
				Matrix<T, 3, 1>(1, 1, 1)
				);
		}

		template <typename T>
		TransformTempl<T>::TransformTempl(void)
		{
		}

		template <typename T>
		TransformTempl<T>::TransformTempl(const Matrix<T, 3, 1> &position,
			const QuaternionTempl<T> &orientation,
			const Matrix<T, 3, 1> &scale) :
		m_position(position),
		m_orientation(orientation),
		m_scale(scale)
		{

		}

		template <typename T>
		TransformTempl<T>::~TransformTempl(void)
		{
		}

		
		template <typename T>
		const QuaternionTempl<T>& TransformTempl<T>::GetOrientation(void) const
		{
			return m_orientation;
		}

		template <typename T>
		void TransformTempl<T>::SetOrientation(const QuaternionTempl<T> &q)
		{
			m_orientation = q;
		}

		template <typename T>
		QuaternionTempl<T>& TransformTempl<T>::Orientation(void)
		{
			return m_orientation;
		}

		template <typename T>
		const Matrix<T, 3, 1>& TransformTempl<T>::GetPosition(void) const
		{
			return m_position;
		}

		template <typename T>
		void TransformTempl<T>::SetPosition(const Matrix<T, 3, 1> &position)
		{
			m_position = position;
		}

		template <typename T>
		Matrix<T, 3, 1>& TransformTempl<T>::Position(void)
		{
			return m_position;
		}

		template <typename T>
		const Matrix<T, 3, 1>& TransformTempl<T>::GetScale(void) const
		{
			return m_scale;
		}

		template <typename T>
		void TransformTempl<T>::SetScale(const Matrix<T, 3, 1> &scale)
		{
			m_scale = scale;
		}

		template <typename T>
		Matrix<T, 3, 1>& TransformTempl<T>::Scale(void)
		{
			return m_scale;
		}

		template <typename T>
		Matrix<T, 4, 4> TransformTempl<T>::GetSRTMatrix(void) const
		{
			return TranslationMatrix4(m_position) *
				RotationMatrix4(m_orientation) *
				ScaleMatrix4(m_scale);
		}

	}

}