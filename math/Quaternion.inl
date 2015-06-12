namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T>::QuaternionTempl(void)
		{
		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T>::QuaternionTempl(T w, T x, T y, T z)
		{
			m_data[0] = x;
			m_data[1] = y;
			m_data[2] = z;
			m_data[3] = w;
		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T>::QuaternionTempl(const Matrix<T, 3, 1> & axis, T angle)
		{
			
			float angleRadians = Radians(angle);
			float sinHalfAngle = Sine(angleRadians * T(0.5));

			m_data[0] = axis.x() * sinHalfAngle;
			m_data[1] = axis.y() * sinHalfAngle;
			m_data[2] = axis.z() * sinHalfAngle;
			
			m_data[3] = Cosine(angleRadians * T(0.5));

		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T>::QuaternionTempl(const Matrix<T, 3, 3> & A)
		{

			m_data[3] = Sqrt(1 + A.m00() + A.m11() + A.m22()) * T(0.5);

			float invFourW = T(1) / (T(4) * m_data[3]);

			m_data[0] = (A.m21() - A.m12()) * invFourW;
			m_data[1] = (A.m02() - A.m20()) * invFourW;
			m_data[2] = (A.m10() - A.m01()) * invFourW;

		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T> QuaternionTempl<T>::Inverse(void) const
		{
			
			float invSquaredNorm = 1.0f / (m_data[0] * m_data[0] + m_data[1] * m_data[1] +
				m_data[2] * m_data[2] + m_data[3] * m_data[3]);

			return QuaternionTempl<T>(
				m_data[3] * invSquaredNorm,
				m_data[0] * invSquaredNorm,
				m_data[1] * invSquaredNorm,
				m_data[2] * invSquaredNorm
				);

		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T> QuaternionTempl<T>::Conjugate(void) const
		{
			return QuaternionTempl<T>(
				m_data[3],
				- m_data[0],
				- m_data[1],
				- m_data[2]
				);
		}

		template <typename T>
		__MYE_MATH_INLINE Matrix<T, 3, 1> QuaternionTempl<T>::Rotate(const Matrix<T, 3, 1> & p) const
		{
			QuaternionTempl<T> pQuat(0, p.x(), p.y(), p.z());
			QuaternionTempl<T> r = *this * pQuat * Conjugate();
			return Matrix<T, 3, 1>(
				r.m_data[0],
				r.m_data[1],
				r.m_data[2]
				);
		}

		template <typename T>
		__MYE_MATH_INLINE T QuaternionTempl<T>::Norm(void) const
		{
			return Sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] +
				m_data[2] * m_data[2] + m_data[3] * m_data[3]);
		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T> QuaternionTempl<T>::Normalize(void) const
		{

			QuaternionTempl<T> q;

			float invNorm = T(1) / Norm();

			q.m_data[3] = m_data[3] * invNorm;
			q.m_data[0] = m_data[0] * invNorm;
			q.m_data[1] = m_data[1] * invNorm;
			q.m_data[2] = m_data[2] * invNorm;

			return q;

		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T> QuaternionTempl<T>::operator+ (const QuaternionTempl & q) const
		{
			return QuaternionTempl<T>(
				m_data[3] + q.m_data[3],
				m_data[0] + q.m_data[0],
				m_data[1] + q.m_data[1],
				m_data[2] + q.m_data[2]
			);
		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T> QuaternionTempl<T>::operator- (const QuaternionTempl & q) const
		{
			return QuaternionTempl<T>(
				m_data[3] - q.m_data[3],
				m_data[0] - q.m_data[0],
				m_data[1] - q.m_data[1],
				m_data[2] - q.m_data[2]
				);
		}

		template <typename T>
		__MYE_MATH_INLINE QuaternionTempl<T> QuaternionTempl<T>::operator* (const QuaternionTempl & q) const
		{
			return QuaternionTempl<T>(
				m_data[3] * q.m_data[3] - m_data[0] * q.m_data[0] - m_data[1] * q.m_data[1] - m_data[2] * q.m_data[2],
				m_data[3] * q.m_data[0] + m_data[0] * q.m_data[3] + m_data[1] * q.m_data[2] - m_data[2] * q.m_data[1],
				m_data[3] * q.m_data[1] + m_data[1] * q.m_data[3] + m_data[2] * q.m_data[0] - m_data[0] * q.m_data[2],
				m_data[3] * q.m_data[2] + m_data[2] * q.m_data[3] + m_data[0] * q.m_data[1] - m_data[1] * q.m_data[0]
			);
		}

		template <typename T>
		__MYE_MATH_INLINE T & QuaternionTempl<T>::operator[] (int i)
		{
			return m_data[i];
		}

		template <typename T>
		__MYE_MATH_INLINE const T & QuaternionTempl<T>::operator[] (int i) const
		{
			return m_data[i];
		}

		template <typename T>
		__MYE_MATH_INLINE T & QuaternionTempl<T>::x(void)
		{
			return m_data[0];
		}

		template <typename T>
		__MYE_MATH_INLINE const T & QuaternionTempl<T>::x(void) const
		{
			return m_data[0];
		}

		template <typename T>
		__MYE_MATH_INLINE T & QuaternionTempl<T>::y(void)
		{
			return m_data[1];
		}

		template <typename T>
		__MYE_MATH_INLINE const T & QuaternionTempl<T>::y(void) const
		{
			return m_data[1];
		}

		template <typename T>
		__MYE_MATH_INLINE T & QuaternionTempl<T>::z(void)
		{
			return m_data[2];
		}

		template <typename T>
		__MYE_MATH_INLINE const T & QuaternionTempl<T>::z(void) const
		{
			return m_data[2];
		}

		template <typename T>
		__MYE_MATH_INLINE T & QuaternionTempl<T>::w(void)
		{
			return m_data[3];
		}

		template <typename T>
		__MYE_MATH_INLINE const T & QuaternionTempl<T>::w(void) const
		{
			return m_data[3];
		}

	}

}