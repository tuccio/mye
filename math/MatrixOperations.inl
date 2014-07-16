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
				x.x() * A(0, 0) + x.y() * A(0, 1) + x.z() * A(0, 2) + A(0, 3),
				x.x() * A(1, 0) + x.y() * A(1, 1) + x.z() * A(1, 2) + A(1, 3),
				x.x() * A(2, 0) + x.y() * A(2, 1) + x.z() * A(2, 2) + A(2, 3)
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

		template <typename T>
		Matrix<T, 4, 4> ScaleMatrix4(const Matrix<T, 3, 1> &scale)
		{
			Matrix<T, 4, 4> smatrix(1.0f);
			smatrix.m00() = scale.x();
			smatrix.m11() = scale.y();
			smatrix.m22() = scale.z();
			return smatrix;
		}

		template <typename T>
		Matrix<T, 4, 4> TranslationMatrix4(const Matrix<T, 3, 1> &translation)
		{
			Matrix<T, 4, 4> tmatrix(1.0f);
			tmatrix.m03() = translation.x();
			tmatrix.m13() = translation.y();
			tmatrix.m23() = translation.z();
			return tmatrix;
		}

		/* Equations */

		template <typename T>
		Matrix<T, 3, 1> Cramer(const Matrix<T, 3, 3> &A, const Matrix<T, 3, 1> &b)
		{

			Matrix<T, 3, 1> c[3] =
			{
				A.GetColumn(0),
				A.GetColumn(1),
				A.GetColumn(2)
			};

			T invDet = T(1) / A.Determinant();

			return Matrix<T, 3, 1>(
				invDet * b.Dot(c[1].Cross(c[2])),
				invDet * c[0].Dot(b.Cross(c[2])),
				invDet * c[0].Dot(c[1].Cross(b))
			);

		}

		


	}

}