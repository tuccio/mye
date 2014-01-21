#include <cassert>
#define __MYE_MATRIX_ACCESS(__MATRIX, __COLS, __I, __J) (*(__MATRIX + ((__COLS) * (__I) + (__J))))

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

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			template <int N>
			Matrix<T, ROWS, N> operator* (const Matrix<T, COLS, N> &b) const;

			Matrix<T, ROWS, COLS> operator+ (const Matrix<T, ROWS, COLS> &b) const;
			Matrix<T, ROWS, COLS> operator- (const Matrix<T, ROWS, COLS> &b) const;

			inline T* Data(void);
			inline const T* Data(void) const;

		private:

			T m_data[ROWS * COLS];

		};

	}
}

#include "Matrix.inl"