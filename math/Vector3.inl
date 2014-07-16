namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 3, 1>::Matrix(void)
		{
		}

		template <typename T>
		Matrix<T, 3, 1>::Matrix(T v)
		{
			m_data[0] = v;
			m_data[1] = v;
			m_data[2] = v;
		}

		template <typename T>
		Matrix<T, 3, 1>::Matrix(T x, T y, T z)
		{
			m_data[0] = x;
			m_data[1] = y;
			m_data[2] = z;
		}

		template <typename T>
		Matrix<T, 3, 1>::Matrix(const Matrix<T, 2, 1> &v, T z)
		{
			m_data[0] = v.x();
			m_data[1] = v.y();
			m_data[2] = z;
		}

		template <typename T>
		T& Matrix<T, 3, 1>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 3 && j == 0);
			return m_data[i];
		}

		template <typename T>
		const T& Matrix<T, 3, 1>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 3 && j == 0);
			return m_data[i];
		}

		template <typename T>
		T& Matrix<T, 3, 1>::operator[] (int i)
		{
			assert(i >= 0 && i < 3);
			return m_data[i];
		}

		template <typename T>
		const T& Matrix<T, 3, 1>::operator[] (int i) const
		{
			assert(i >= 0 && i < 3);
			return m_data[i];
		}

		template <typename T>
		T Matrix<T, 3, 1>::Dot(const Matrix<T, 3, 1> &v) const
		{
			return m_data[0] * v.m_data[0] + m_data[1] * v.m_data[1] + m_data[2] * v.m_data[2];
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::Cross(const Matrix<T, 3, 1> &v) const
		{
			return Matrix<T, 3, 1>(
				m_data[1] * v.m_data[2] - m_data[2] * v.m_data[1],
				m_data[2] * v.m_data[0] - m_data[0] * v.m_data[2],
				m_data[0] * v.m_data[1] - m_data[1] * v.m_data[0]);
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::operator+ (const Matrix<T, 3, 1> &b) const
		{
			return Matrix<T, 3, 1>(
				m_data[0] + b.m_data[0],
				m_data[1] + b.m_data[1],
				m_data[2] + b.m_data[2]);
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::operator- (const Matrix<T, 3, 1> &b) const
		{
			return Matrix<T, 3, 1>(
				m_data[0] - b.m_data[0],
				m_data[1] - b.m_data[1],
				m_data[2] - b.m_data[2]);
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::operator* (const Matrix<T, 3, 1> &v) const
		{
			return Matrix<T, 3, 1>(
				m_data[0] * v.m_data[0],
				m_data[1] * v.m_data[1],
				m_data[2] * v.m_data[2]
			);
		}

		template <typename T>
		inline Matrix<T, 3, 1> Matrix<T, 3, 1>::operator/ (const Matrix<T, 3, 1> &v) const
		{
			return Matrix<T, 3, 1>(
				m_data[0] / v.m_data[0],
				m_data[1] / v.m_data[1],
				m_data[2] / v.m_data[2]
			);
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::operator* (T x) const
		{
			return Matrix<T, 3, 1>(
				m_data[0] * x,
				m_data[1] * x,
				m_data[2] * x
				);
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::operator- (void) const
		{
			return Matrix<T, 3, 1>(
				- m_data[0],
				- m_data[1],
				- m_data[2]
			);
		}

		template <typename T>
		Matrix<T, 3, 1>& Matrix<T, 3, 1>::operator+= (const Matrix<T, 3, 1> &v)
		{
			m_data[0] += v.m_data[0];
			m_data[1] += v.m_data[1];
			m_data[2] += v.m_data[2];
			return *this;
		}
		
		template <typename T>
		Matrix<T, 3, 1>& Matrix<T, 3, 1>::operator-= (const Matrix<T, 3, 1> &v)
		{
			m_data[0] -= v.m_data[0];
			m_data[1] -= v.m_data[1];
			m_data[2] -= v.m_data[2];
			return *this;
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::Normalize(void) const
		{
			T norm = Length();
			return Matrix<T, 3, 1>(
				m_data[0] / norm,
				m_data[1] / norm,
				m_data[2] / norm
				);
		}

		template <typename T>
		T Matrix<T, 3, 1>::Length(void) const
		{
			return Sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2]);
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::Reflect(const Matrix<T, 3, 1> &normal) const
		{
			return *this - (2.0f * this->Dot(normal)) * normal;
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::CwiseAbs(void) const
		{
			return Matrix<T, 3, 1>(
				Abs(m_data[0]),
				Abs(m_data[1]),
				Abs(m_data[2])
				);
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::CwiseMin(const Matrix<T, 3, 1> &v) const
		{
			return Matrix<T, 3, 1>(
				Min(m_data[0], v.m_data[0]),
				Min(m_data[1], v.m_data[1]),
				Min(m_data[2], v.m_data[2])
				);
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::CwiseMax(const Matrix<T, 3, 1> &v) const
		{
			return Matrix<T, 3, 1>(
				Max(m_data[0], v.m_data[0]),
				Max(m_data[1], v.m_data[1]),
				Max(m_data[2], v.m_data[2])
				);
		}

		template <typename T>
		T* Matrix<T, 3, 1>::Data(void)
		{
			return m_data;
		}

		template <typename T>
		const T* Matrix<T, 3, 1>::Data(void) const
		{
			return m_data;
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::Clamp(T minimum, T maximum)
		{
			return Clamp(Matrix<T, 3, 1>(minimum), Matrix<T, 3, 1>(maximum));
		}

		template <typename T>
		Matrix<T, 3, 1> Matrix<T, 3, 1>::Clamp(const Matrix<T, 3, 1> &minimum, const Matrix<T, 3, 1> &maximum)
		{

			Matrix<T, 3, 1> v;

			v.m_data[0] = ((m_data[0] < minimum.m_data[0]) ? minimum.m_data[0] : ((m_data[0] > maximum.m_data[0]) ? maximum.m_data[0] : m_data[0]));
			v.m_data[1] = ((m_data[1] < minimum.m_data[1]) ? minimum.m_data[1] : ((m_data[1] > maximum.m_data[1]) ? maximum.m_data[1] : m_data[1]));
			v.m_data[2] = ((m_data[2] < minimum.m_data[2]) ? minimum.m_data[2] : ((m_data[2] > maximum.m_data[2]) ? maximum.m_data[2] : m_data[2]));

			return v;

		}

		/* Operators */

		template <typename T>
		inline bool operator== (const Matrix<T, 3, 1> &a,
			const Matrix<T, 3, 1> &b)
		{
			return a.x() == b.x() &&
				a.y() == b.y() &&
				a.z() == b.z();
		}

		template <typename T>
		Matrix<T, 3, 1> operator+ (T a, const Matrix<T, 3, 1> &b)
		{
			return Matrix<T, 3, 1>(a + b.x(), a + b.y(), a + b.z());
		}

		template <typename T>
		Matrix<T, 3, 1> operator- (T a, const Matrix<T, 3, 1> &b)
		{
			return Matrix<T, 3, 1>(a - b.x(), a - b.y(), a - b.z());
		}

		template <typename T>
		Matrix<T, 3, 1> operator* (T a, const Matrix<T, 3, 1> &b)
		{
			return Matrix<T, 3, 1>(a * b.x(), a * b.y(), a * b.z());
		}

		template <typename T>
		Matrix<T, 3, 1> operator/ (T a, const Matrix<T, 3, 1> &b)
		{
			return Matrix<T, 3, 1>(a / b.x(), a / b.y(), a / b.z());
		}

	}

}