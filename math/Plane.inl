namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T>::PlaneTempl(void)
		{
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T>::PlaneTempl(const Matrix<T, 3, 1> & p, const Matrix<T, 3, 1> & N) :
			m_plane(N, - p.Dot(N))
		{
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T>::PlaneTempl(const Matrix<T, 3, 1> & N, T d) :
			m_plane(N, d)
		{
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T>::PlaneTempl(T a, T b, T c, T d) :
			m_plane(a, b, c, d)
		{
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T>::PlaneTempl(const Matrix<T, 4, 1> & plane) :
			m_plane(plane)
		{
		}

		template <typename T>
		__MYE_MATH_INLINE bool PlaneTempl<T>::Contains(const Matrix<T, 3, 1> & x) const
		{
			return m_plane.xyz().Dot(x) == T(0);
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
		__MYE_MATH_INLINE T PlaneTempl<T>::Distance(const Matrix<T, 3, 1> & x) const
		{
			return Normalize().DistanceNormalized(x);
		}

		template <typename T>
		__MYE_MATH_INLINE T PlaneTempl<T>::DistanceNormalized(const Matrix<T, 3, 1> & x) const
		{
			Matrix<T, 4, 1> v(x, 1);
			return v.Dot(m_plane);
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 3, 1> PlaneTempl<T>::Normal(void) const
		{
			return m_plane.xyz();
		}

		template <typename T>
		__MYE_MATH_INLINE T PlaneTempl<T>::Coefficient(void) const
		{
			return m_plane.w();
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 4, 1> PlaneTempl<T>::Parameters(void) const
		{
			return m_plane;
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T> PlaneTempl<T>::Normalize(void) const
		{
			Matrix<T, 3, 1> N = m_plane.xyz();
			Matrix<T, 4, 1> plane = m_plane / N.Length();
			return PlaneTempl<T>(plane);
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T> PlaneTempl<T>::Flip(void) const
		{
			return PlaneTempl<T>(-m_plane.xyz(), m_plane.w());
		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T> PlaneTempl<T>::Transform(const Matrix<T, 4, 4> & t) const
		{

			Matrix<T, 3, 1> N = m_plane.xyz();
			Matrix<T, 4, 1> o = Vector4(N * m_plane.www(), 1);
			Matrix<T, 4, 1> N2 = Vector4(N, 0);

			o = t * o;
			N2 = t.Inverse().Transpose() * N2;

			return PlaneTempl<T>(-o.xyz(), N2.xyz());

		}

		template <typename T>
		__MYE_MATH_INLINE PlaneTempl<T> PlaneTempl<T>::TranslateAlongNormal(float delta) const
		{
			Matrix<T, 3, 1> N = m_plane.xyz();
			Matrix<T, 4, 1> diff(0, 0, 0, delta * N.Dot(N));
			return PlaneTempl<T>(m_plane - diff);
		}

	}

}