namespace mye
{

	namespace math
	{

		/* Matrix vector */

		template <typename T>
		Matrix<T, 3, 1> operator* (const Matrix<T, 3, 3> & A, const Matrix<T, 3, 1> & x)
		{
			return Matrix<T, 3, 1>(
				x.x() * A(0, 0) + x.y() * A(0, 1) + x.z() * A(0, 2),
				x.x() * A(1, 0) + x.y() * A(1, 1) + x.z() * A(1, 2),
				x.x() * A(2, 0) + x.y() * A(2, 1) + x.z() * A(2, 2)
				);
		}

		template <typename T>
		Matrix<T, 3, 1> operator* (const Matrix<T, 4, 4> & A, const Matrix<T, 3, 1> & x)
		{
			return Matrix<T, 3, 1>(
				x.x() * A(0, 0) + x.y() * A(0, 1) + x.z() * A(0, 2) + A(0, 3),
				x.x() * A(1, 0) + x.y() * A(1, 1) + x.z() * A(1, 2) + A(1, 3),
				x.x() * A(2, 0) + x.y() * A(2, 1) + x.z() * A(2, 2) + A(2, 3)
				);
		}

		template <typename T>
		Matrix<T, 4, 1> operator* (const Matrix<T, 4, 3> & A, const Matrix<T, 4, 1> & x)
		{
			return Matrix<T, 4, 1>(
				x.x() * A(0, 0) + x.y() * A(0, 1) + x.z() * A(0, 2) + x.w() * A(0, 3),
				x.x() * A(1, 0) + x.y() * A(1, 1) + x.z() * A(1, 2) + x.w() * A(1, 3),
				x.x() * A(2, 0) + x.y() * A(2, 1) + x.z() * A(2, 2) + x.w() * A(2, 3),
				x.x() * A(3, 0) + x.y() * A(3, 1) + x.z() * A(3, 2) + x.w() * A(3, 3)
				);
		}

		template <typename T>
		Matrix<T, 4, 4> ScaleMatrix4(const Matrix<T, 3, 1> & scale)
		{

			Matrix<T, 4, 4> m = {
				scale.x(), 0.0f, 0.0f, 0.0f,
				0.0f, scale.y(), 0.0f, 0.0f,
				0.0f, 0.0f, scale.z(), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};

			return m;

		}

		template <typename T>
		Matrix<T, 4, 4> TranslationMatrix4(const Matrix<T, 3, 1> & translation)
		{

			Matrix<T, 4, 4> m = {
				1.0f, 0.0f, 0.0f, translation.x(),
				0.0f, 1.0f, 0.0f, translation.y(),
				0.0f, 0.0f, 1.0f, translation.z(),
				0.0f, 0.0f, 0.0f, 1.0f
			};

			return m;

		}

		/* Equations */

		template <typename T>
		Matrix<T, 3, 1> Cramer(const Matrix<T, 3, 3> & A, const Matrix<T, 3, 1> & b)
		{

			Matrix<T, 3, 3> At = A.Transpose();

			Matrix<T, 3, 1> c[3] =
			{
				A.GetRow(0),
				A.GetRow(1),
				A.GetRow(2)
			};

			T invDet = T(1) / A.Determinant();

			return invDet * Matrix<T, 3, 1>(b.Dot(c[1].Cross(c[2])),
			                                c[0].Dot(b.Cross(c[2])),
			                                c[0].Dot(c[1].Cross(b)));

		}

		


	}

}