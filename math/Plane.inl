namespace mye
{

	namespace math
	{

		template <typename T>
		Plane<T>::Plane(void)
		{
		}

		template <typename T>
		Plane<T>::Plane(const Matrix<T, 3, 1> &p,
			const Matrix<T, 3, 1> &n) :
			m_normal(n),
			m_coefficient(- p.Dot(n))
		{
		}

		template <typename T>
		Plane<T>::Plane(T a, T b, T c, T d) :
			m_plane(a, b, c, d)
		{
		}

		template <typename T>
		bool Plane<T>::Contains(const Matrix<T, 3, 1> &x) const
		{
			return m_normal.Dot(x) == T(0);
		}

		template <typename T>
		PlaneSide Plane<T>::Side(const Matrix<T, 3, 1> &x) const
		{

			T dot = m_normal.Dot(x) + m_plane.w();

			if (dot == 0)
			{
				return PlaneSide::INSIDE;
			}
			else if (dot > 0)
			{
				return PlaneSide::FRONT;
			}
			else
			{
				return PlaneSide::BACK;
			}

		}

		template <typename T>
		const Matrix<T, 3, 1>& Plane<T>::Normal(void) const
		{
			return m_normal;
		}

		template <typename T>
		const T& Plane<T>::Coefficient(void) const
		{
			return m_coefficient;
		}

	}

}