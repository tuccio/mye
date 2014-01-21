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

	}

}