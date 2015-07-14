#pragma once

#include <cassert>
#include <type_traits>

#define __MYE_MATRIX_ACCESS(__MATRIX, __COLS, __I, __J) (__MATRIX[__I * __COLS + __J])

namespace mye
{
	namespace math
	{

		template <typename T, int ROWS, int COLS>
		class Matrix
		{

		public:

			Matrix(void);

			void Fill(T x);

			inline T &       operator() (int i, int j);
			inline const T & operator() (int i, int j) const;

			template <int N>
			Matrix<T, ROWS, N> operator* (const Matrix<T, COLS, N> & b) const;

			Matrix<T, ROWS, COLS> operator+ (const Matrix<T, ROWS, COLS> & b) const;
			Matrix<T, ROWS, COLS> operator- (const Matrix<T, ROWS, COLS> & b) const;

			bool operator== (const Matrix<T, ROWS, COLS> & b) const;

			inline T *       Data(void);
			inline const T * Data(void) const;

		private:

			T m_data[ROWS * COLS];

		};

		
		template <typename T, int N, int M>
		struct MatrixTemplateInfo
		{
			typedef T Scalar;
			typedef std::integral_constant<int, N> Rows;
			typedef std::integral_constant<int, M> Cols;
		};

		template <typename T, int N, int M>
		__MYE_MATH_INLINE MatrixTemplateInfo<T, N, M> DeduceMatrixTemplateInfo(const Matrix<T, N, M> &)
		{
			return MatrixTemplateInfo<T, N, M>();
		}

		template <typename T, int N, int M>
		__MYE_MATH_INLINE T DeduceMatrixScalar(const Matrix<T, N, M> &)
		{
			return T(0);
		}

	}
}

#include "Matrix.inl"