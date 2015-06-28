namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE SphereTempl<T>::SphereTempl(void)
		{

		}

		template <typename T>
		__MYE_MATH_INLINE SphereTempl<T>::SphereTempl(const Matrix<T, 3, 1> & center, T radius) :
			m_sphere(center, radius)
		{

		}

		template <typename T>
		__MYE_MATH_INLINE const Matrix<T, 3, 1> SphereTempl<T>::GetCenter(void) const
		{
			return m_sphere.xyz();
		}

		template <typename T>
		__MYE_MATH_INLINE void SphereTempl<T>::SetCenter(const Matrix<T, 3, 1> & center)
		{
			m_sphere = Matrix<T, 4, 1>(center, m_sphere.w());
		}

		template <typename T>
		__MYE_MATH_INLINE T SphereTempl<T>::GetRadius(void) const
		{
			return m_sphere.w();
		}

		template <typename T>
		__MYE_MATH_INLINE void SphereTempl<T>::SetRadius(T radius)
		{
			m_sphere.w() = radius;
		}

	}

}