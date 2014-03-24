namespace mye
{

	namespace math
	{

		template <typename T>
		PlaneTempl<T>::PlaneTempl(void)
		{
		}

		template <typename T>
		PlaneTempl<T>::PlaneTempl(const Matrix<T, 3, 1> &p,
			const Matrix<T, 3, 1> &n) :
			m_normal(n),
			m_coefficient(- p.Dot(n))
		{
		}

		template <typename T>
		PlaneTempl<T>::PlaneTempl(T a, T b, T c, T d) :
			m_normal(a, b, c),
			m_coefficient(d)
		{
		}

		template <typename T>
		bool PlaneTempl<T>::Contains(const Matrix<T, 3, 1> &x) const
		{
			return m_normal.Dot(x) == T(0);
		}

		template <typename T>
		PlanetSide PlaneTempl<T>::Side(const Matrix<T, 3, 1> &x) const
		{

			T dot = m_normal.Dot(x) + m_Planet.w();

			if (dot == 0)
			{
				return PlanetSide::INSIDE;
			}
			else if (dot > 0)
			{
				return PlanetSide::FRONT;
			}
			else
			{
				return PlanetSide::BACK;
			}

		}

		template <typename T>
		const Matrix<T, 3, 1>& PlaneTempl<T>::Normal(void) const
		{
			return m_normal;
		}

		template <typename T>
		const T& PlaneTempl<T>::Coefficient(void) const
		{
			return m_coefficient;
		}

		template <typename T>
		PlaneTempl<T> PlaneTempl<T>::Transformt(const Matrix<T, 4, 4> &TransformTempl) const
		{
			Matrix<T, 4, 1> p(m_normal, m_coefficient);
			Matrix<T, 4, 1> pt = TransformTempl.Inverse().Transpose() * p;
			return PlaneTempl<T>(p.x(), p.y(), p.z(), p.w());
		}

	}

}