namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T>::PlaneTempl(void)
		{
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T>::PlaneTempl(const Matrix<T, 3, 1> & p,
			const Matrix<T, 3, 1> & n) :
			m_plane(n, -p.Dot(n))
		{
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T>::PlaneTempl(T a, T b, T c, T d) :
			m_plane(a, b, c, d)
		{
		}

		template <typename T>
		__MYE_MATH_INLINE bool PlaneTempl<T>::Contains(const Matrix<T, 3, 1> & x) const
		{
			return m_normal.Dot(x) == T(0);
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneSide PlaneTempl<T>::Side(const Matrix<T, 3, 1> & x) const
		{

			T dot = m_plane.Dot(Matrix<T, 4, 1>(x));

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
		__MYE_MATH_INLINE Matrix<T, 3, 1> PlaneTempl<T>::Normal(void) const
		{
			return m_plane.xyz();
		}

		template <typename T>
		__MYE_MATH_INLINE const T & PlaneTempl<T>::Coefficient(void) const
		{
			return m_coefficient;
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T> PlaneTempl<T>::Transform(const Matrix<T, 4, 4> & t) const
		{
			Matrix<T, 4, 1> p(m_normal, m_coefficient);
			Matrix<T, 4, 1> pt = TransformTempl.Inverse().Transpose() * p;
			return PlaneTempl<T>(p.x(), p.y(), p.z(), p.w());
		}

	}

}