namespace mye
{

	namespace math
	{

		template <typename T>
		Quaterniont<T>::Quaterniont(void)
		{
		}

		template <typename T>
		Quaterniont<T>::Quaterniont(T w, T x, T y, T z)
		{
			m_data[0] = x;
			m_data[1] = y;
			m_data[2] = z;
			m_data[3] = w;
		}

		template <typename T>
		Quaterniont<T>::Quaterniont(const Matrix<T, 3, 1> &axis, T angle)
		{
			float angleRadians = Radians(angle);
			float sinHalfAngle = Sine(angleRadians * 0.5f);
			m_data[3] = Cosine(angleRadians * 0.5f);
			m_data[0] = axis.x() * sinHalfAngle;
			m_data[1] = axis.y() * sinHalfAngle;
			m_data[2] = axis.z() * sinHalfAngle;
		}

		template <typename T>
		Quaterniont<T>::Quaterniont(const Matrix<T, 3, 3> &A)
		{
			m_data[3] = Sqrt(1 + A(0, 0) + A(1, 1) + A(2, 2)) * 0.5f;
			float invFourW = 1.0f / (4.0f * m_data[3]);
			m_data[0] = (A(2, 1) - A(1, 2)) * invFourW;
			m_data[1] = (A(0, 2) - A(2, 0)) * invFourW;
			m_data[2] = (A(1, 0) - A(0, 1)) * invFourW;
		}

		template <typename T>
		Quaterniont<T> Quaterniont<T>::Inverse(void) const
		{
			
			float invSquaredNorm = 1.0f / (m_data[0] * m_data[0] + m_data[1] * m_data[1] +
				m_data[2] * m_data[2] + m_data[3] * m_data[3]);

			return Quaterniont<T>(
				m_data[3] * invSquaredNorm,
				m_data[0] * invSquaredNorm,
				m_data[1] * invSquaredNorm,
				m_data[2] * invSquaredNorm
				);

		}

		template <typename T>
		Quaterniont<T> Quaterniont<T>::Conjugate(void) const
		{
			return Quaterniont<T>(
				m_data[3],
				- m_data[0],
				- m_data[1],
				- m_data[2]
				);
		}

		template <typename T>
		Matrix<T, 3, 1> Quaterniont<T>::Rotate(const Matrix<T, 3, 1> &p) const
		{
			Quaterniont<T> pQuat(0, p.x(), p.y(), p.z());
			Quaterniont<T> r = *this * pQuat * Conjugate();
			return Matrix<T, 3, 1>(
				r.m_data[0],
				r.m_data[1],
				r.m_data[2]
				);
		}

		template <typename T>
		T Quaterniont<T>::Norm(void) const
		{
			return Sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] +
				m_data[2] * m_data[2] + m_data[3] * m_data[3]);
		}

		template <typename T>
		Quaterniont<T>& Quaterniont<T>::Normalize(void)
		{
			float invNorm = 1.0f / Norm();

			m_data[3] *= invNorm;
			m_data[0] *= invNorm;
			m_data[1] *= invNorm;
			m_data[2] *= invNorm;
		}

		template <typename T>
		Quaterniont<T> Quaterniont<T>::Normalized(void) const
		{
			float invNorm = 1.0f / Norm();

			return Quaterniont<T>(
				m_data[3] * invNorm,
				m_data[0] * invNorm,
				m_data[1] * invNorm,
				m_data[2] * invNorm
				);
		}

		template <typename T>
		Quaterniont<T> Quaterniont<T>::operator+ (const Quaterniont &q) const
		{
			return Quaterniont<T>(
				m_data[3] + q.m_data[3],
				m_data[0] + q.m_data[0],
				m_data[1] + q.m_data[1],
				m_data[2] + q.m_data[2]
			);
		}

		template <typename T>
		Quaterniont<T> Quaterniont<T>::operator- (const Quaterniont &q) const
		{
			return Quaterniont<T>(
				m_data[3] - q.m_data[3],
				m_data[0] - q.m_data[0],
				m_data[1] - q.m_data[1],
				m_data[2] - q.m_data[2]
				);
		}

		template <typename T>
		Quaterniont<T> Quaterniont<T>::operator* (const Quaterniont &q) const
		{
			return Quaterniont<T>(
				m_data[3] * q.m_data[3] - m_data[0] * q.m_data[0] - m_data[1] * q.m_data[1] - m_data[2] * q.m_data[2],
				m_data[3] * q.m_data[0] + m_data[0] * q.m_data[3] + m_data[1] * q.m_data[2] - m_data[2] * q.m_data[1],
				m_data[3] * q.m_data[1] + m_data[1] * q.m_data[3] + m_data[2] * q.m_data[0] - m_data[0] * q.m_data[2],
				m_data[3] * q.m_data[2] + m_data[2] * q.m_data[3] + m_data[0] * q.m_data[1] - m_data[1] * q.m_data[0]
			);
		}

		template <typename T>
		T& Quaterniont<T>::operator[] (int i)
		{
			return m_data[i];
		}

		template <typename T>
		const T& Quaterniont<T>::operator[] (int i) const
		{
			return m_data[i];
		}

		template <typename T>
		T& Quaterniont<T>::x(void)
		{
			return m_data[0];
		}

		template <typename T>
		const T& Quaterniont<T>::x(void) const
		{
			return m_data[0];
		}

		template <typename T>
		T& Quaterniont<T>::y(void)
		{
			return m_data[1];
		}

		template <typename T>
		const T& Quaterniont<T>::y(void) const
		{
			return m_data[1];
		}

		template <typename T>
		T& Quaterniont<T>::z(void)
		{
			return m_data[2];
		}

		template <typename T>
		const T& Quaterniont<T>::z(void) const
		{
			return m_data[2];
		}

		template <typename T>
		T& Quaterniont<T>::w(void)
		{
			return m_data[3];
		}

		template <typename T>
		const T& Quaterniont<T>::w(void) const
		{
			return m_data[3];
		}

	}

}