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
		inline Matrix<T, 3, 1> operator* (const Matrix<T, 3, 3> &A, const Matrix<T, 3, 1> &x);

		template <typename T>
		inline Matrix<T, 3, 1> operator* (const Matrix<T, 4, 4> &A, const Matrix<T, 3, 1> &x);

		template <typename T>
		inline Matrix<T, 4, 1> operator* (const Matrix<T, 4, 3> &A, const Matrix<T, 4, 1> &x);

	}

}

#include "MatrixOperations.inl"