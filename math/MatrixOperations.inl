namespace mye
{

	namespace math
	{

		/* Equals */

		template <typename T, int ROWS, int COLS>
		bool operator== (const Matrix<T, ROWS, COLS> &a,
			const Matrix<T, ROWS, COLS> &b)
		{

			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					if (a(i, j) != b(i, j))
					{
						return false;
					}
				}
			}

			return true;

		}

		template <typename T>
		inline bool operator== (const Matrix<T, 2, 1> &a,
			const Matrix<T, 2, 1> &b)
		{
			return a.x() == b.x() &&
				a.y() == b.y();
		}

		template <typename T>
		inline bool operator== (const Matrix<T, 3, 1> &a,
			const Matrix<T, 3, 1> &b)
		{
			return a.x() == b.x() &&
				a.y() == b.y() &&
				a.z() == b.z();
		}

		template <typename T>
		inline bool operator== (const Matrix<T, 4, 1> &a,
			const Matrix<T, 4, 1> &b)
		{
			return a.x() == b.x() &&
				a.y() == b.y() &&
				a.z() == b.z() &&
				a.w() == b.w();
		}

		/* Matrix vector */

		template <typename T>
		Matrix<T, 3, 1> operator* (const Matrix<T, 3, 3> &A, const Matrix<T, 3, 1> &x)
		{
			return Matrix<T, 3, 1>(
				x.x() * A(0, 0) + x.y() * A(0, 1) + x.z() * A(0, 2),
				x.x() * A(1, 0) + x.y() * A(1, 1) + x.z() * A(1, 2),
				x.x() * A(2, 0) + x.y() * A(2, 1) + x.z() * A(2, 2)
				);
		}

		template <typename T>
		Matrix<T, 3, 1> operator* (const Matrix<T, 4, 4> &A, const Matrix<T, 3, 1> &x)
		{
			return Matrix<T, 3, 1>(
				x.x() * A(0, 0) + x.y() * A(0, 1) + x.z() * A(0, 2),
				x.x() * A(1, 0) + x.y() * A(1, 1) + x.z() * A(1, 2),
				x.x() * A(2, 0) + x.y() * A(2, 1) + x.z() * A(2, 2)
				);
		}

		template <typename T>
		Matrix<T, 4, 1> operator* (const Matrix<T, 4, 3> &A, const Matrix<T, 4, 1> &x)
		{
			return Matrix<T, 4, 1>(
				x.x() * A(0, 0) + x.y() * A(0, 1) + x.z() * A(0, 2) + x.w() * A(0, 3),
				x.x() * A(1, 0) + x.y() * A(1, 1) + x.z() * A(1, 2) + x.w() * A(1, 3),
				x.x() * A(2, 0) + x.y() * A(2, 1) + x.z() * A(2, 2) + x.w() * A(2, 3),
				x.x() * A(3, 0) + x.y() * A(3, 1) + x.z() * A(3, 2) + x.w() * A(3, 3)
				);
		}

		/* Matrix scalar */

		template <typename T>
		Matrix<T, 2, 1> operator* (T s, const Matrix<T, 2, 1> &v)
		{
			return Matrix<T, 2, 1>(
				s * v.x(),
				s * v.y()
				);
		}

		template <typename T>
		Matrix<T, 3, 1> operator* (T s, const Matrix<T, 3, 1> &v)
		{
			return Matrix<T, 3, 1>(
				s * v.x(),
				s * v.y(),
				s * v.z()
				);
		}

		template <typename T>
		Matrix<T, 4, 1> operator* (T s, const Matrix<T, 4, 1> &v)
		{
			return Matrix<T, 4, 1>(
				s * v.x(),
				s * v.y(),
				s * v.z(),
				s * v.w()
				);
		}

		template <typename T>
		Matrix<T, 4, 4> ScaleMatrix4(const Matrix<T, 3, 1> &scale)
		{
			Matrix<T, 4, 4> smatrix(1.0f);
			smatrix(0, 0) = scale.x();
			smatrix(1, 1) = scale.y();
			smatrix(2, 2) = scale.z();
			return smatrix;
		}

		template <typename T>
		Matrix<T, 4, 4> TranslationMatrix4(const Matrix<T, 3, 1> &translation)
		{
			Matrix<T, 4, 4> tmatrix(1.0f);
			tmatrix(0, 3) = translation.x();
			tmatrix(1, 3) = translation.y();
			tmatrix(2, 3) = translation.z();
			return tmatrix;
		}

	}

}