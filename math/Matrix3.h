#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 3, 3>
		{

		public:

			Matrix(void);
			Matrix(const Matrix<T, 4, 4> &m);
			Matrix(T d);

			inline void Fill(T x);

			inline Matrix<T, 3, 1> GetRow(int i) const;
			inline void SetRow(int i, const Matrix<T, 3, 1> &v);

			inline Matrix<T, 3, 1> GetColumn(int j) const;
			inline void SetColumn(int j, const Matrix<T, 3, 1> &v);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline Matrix<T, 3, 3> operator+ (const Matrix<T, 3, 3> &a) const;

			inline Matrix<T, 3, 3> operator* (const Matrix<T, 3, 3> &a) const;
			inline Matrix<T, 3, 3> operator* (T s) const;

			inline Matrix<T, 3, 3>& ScaleInPlace(T s);

			inline Matrix<T, 3, 3> Transpose(void) const;
			inline T Determinant(void) const;
			inline Matrix<T, 3, 3> Inverse(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;

#define __MYE_MATH_DEFINE_MATRIX3_GET_SET(N, I, AUX) \
	inline T BOOST_PP_CAT(Get, BOOST_PP_CAT(BOOST_PP_DIV(I, 3), BOOST_PP_MOD(I, 3))) (void) const { return *(m_data + I); }\
	inline void BOOST_PP_CAT(Set, BOOST_PP_CAT(BOOST_PP_DIV(I, 3), BOOST_PP_MOD(I, 3))) (const T &x) { *(m_data + I) = x; }

#define __MYE_MATH_DEFINE_MATRIX3_REFS(N, I, AUX) \
	inline const T& BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 3), BOOST_PP_MOD(I, 3))) (void) const { return *(m_data + I); }\
	inline T& BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 3), BOOST_PP_MOD(I, 3))) (void) { return *(m_data + I); }

			BOOST_PP_REPEAT(9,
				__MYE_MATH_DEFINE_MATRIX3_GET_SET,
				0)

			BOOST_PP_REPEAT(9,
				__MYE_MATH_DEFINE_MATRIX3_REFS,
				0)

		private:

			T m_data[9];

		};

		template <typename T>
		inline Matrix<T, 3, 3> operator* (T s, const Matrix<T, 3, 3> &m);

	}

}

#include "Matrix3.inl"