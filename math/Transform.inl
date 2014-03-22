namespace mye
{

	namespace math
	{

		template <typename T>
		Transformt<T> Transformt<T>::Identity(void)
		{
			return Transformt<T>(
				Matrix<T, 3, 1>(0, 0, 0),
				Quaterniont<T>(1, 0, 0, 0),
				Matrix<T, 3, 1>(1, 1, 1)
				);
		}

		template <typename T>
		Transformt<T>::Transformt(void)
		{
		}

		template <typename T>
		Transformt<T>::Transformt(const Matrix<T, 3, 1> &position,
			const Quaterniont<T> &orientation,
			const Matrix<T, 3, 1> &scale) :
		m_position(position),
		m_orientation(orientation),
		m_scale(scale)
		{

		}

		template <typename T>
		Transformt<T>::~Transformt(void)
		{
		}

		
		template <typename T>
		const Quaterniont<T>& Transformt<T>::GetOrientation(void) const
		{
			return m_orientation;
		}

		template <typename T>
		void Transformt<T>::SetOrientation(const Quaterniont<T> &q)
		{
			m_orientation = q;
		}

		template <typename T>
		const Matrix<T, 3, 1>& Transformt<T>::GetPosition(void) const
		{
			return m_position;
		}

		template <typename T>
		void Transformt<T>::SetPosition(const Matrix<T, 3, 1> &position)
		{
			m_position = position;
		}

		template <typename T>
		const Matrix<T, 3, 1>& Transformt<T>::GetScale(void) const
		{
			return m_scale;
		}

		template <typename T>
		void Transformt<T>::SetScale(const Matrix<T, 3, 1> &scale)
		{
			m_scale = scale;
		}

		template <typename T>
		Matrix<T, 4, 4> Transformt<T>::GetSRTMatrix(void) const
		{
			return TranslationMatrix4(m_position) *
				RotationMatrix4(m_orientation) *
				ScaleMatrix4(m_scale);
		}

	}

}