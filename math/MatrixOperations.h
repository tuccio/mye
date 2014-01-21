#pragma once

namespace mye
{

	namespace math
	{

		/* Equals */

		template <typename T, int ROWS, int COLS>
		bool operator== (const Matrix<T, ROWS, COLS> &a,
			const Matrix<T, ROWS, COLS> &b);

		template <typename T>
		inline bool operator== (const Matrix<T, 2, 1> &a,
			const Matrix<T, 2, 1> &b);

		template <typename T>
		inline bool operator== (const Matrix<T, 3, 1> &a,
			const Matrix<T, 3, 1> &b);

		template <typename T>
		inline bool operator== (const Matrix<T, 4, 1> &a,
			const Matrix<T, 4, 1> &b);

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
		Matrix<T, 4, 1> operator* (const Matrix<T, 4, 3> &A, const Matrix<T, 4, 1> &x)
		{
			return Matrix<T, 4, 1>(
				x.x() * A(0, 0) + x.y() * A(0, 1) + x.z() * A(0, 2) + x.w() * A(0, 3),
				x.x() * A(1, 0) + x.y() * A(1, 1) + x.z() * A(1, 2) + x.w() * A(1, 3),
				x.x() * A(2, 0) + x.y() * A(2, 1) + x.z() * A(2, 2) + x.w() * A(2, 3),
				x.x() * A(3, 0) + x.y() * A(3, 1) + x.z() * A(3, 2) + x.w() * A(3, 3)
				);
		}

	}

}

#include "MatrixOperations.inl"