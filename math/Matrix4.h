#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Matrix<T, 4, 4>
		{

		public:

			Matrix(void);
			Matrix(T d);
			Matrix(const Matrix<T, 3, 3> &m);

			inline void Fill(T x);

			inline Matrix<T, 4, 1> GetRow(int i) const;
			inline void SetRow(int i, const Matrix<T, 4, 1> &v);

			inline Matrix<T, 4, 1> GetColumn(int i) const;
			inline void SetColumn(int i, const Matrix<T, 4, 1> &v);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

			inline Matrix<T, 4, 4>& ScaleInPlace(T s);

			inline Matrix<T, 4, 4>& operator= (const Matrix<T, 4, 4> &m);

			inline Matrix<T, 4, 4> operator* (const Matrix<T, 4, 4> &a) const;
			inline Matrix<T, 4, 4> operator* (T s) const;

			inline Matrix<T, 4, 4> operator+ (const Matrix<T, 4, 4> &a) const;

			inline Matrix<T, 4, 4> operator- (void) const;

			inline Matrix<T, 4, 4> Transpose(void) const;
			inline Matrix<T, 4, 4> Inverse(void) const;
			inline T Determinant(void) const;

			inline T* Data(void);
			inline const T* Data(void) const;

			// Define Get/Set ij functions ...
			// Define m ij functions ...

#define __MYE_MATH_DEFINE_MATRIX4_GET_SET(N, I, AUX) \
	inline T BOOST_PP_CAT(Get, BOOST_PP_CAT(BOOST_PP_DIV(I, 4), BOOST_PP_MOD(I, 4))) (void) const { return *(m_data + I); }\
	inline void BOOST_PP_CAT(Set, BOOST_PP_CAT(BOOST_PP_DIV(I, 4), BOOST_PP_MOD(I, 4))) (const T &x) { *(m_data + I) = x; }

#define __MYE_MATH_DEFINE_MATRIX4_REFS(N, I, AUX) \
	inline const T& BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 4), BOOST_PP_MOD(I, 4))) (void) const { return *(m_data + I); }\
	inline T& BOOST_PP_CAT(m, BOOST_PP_CAT(BOOST_PP_DIV(I, 4), BOOST_PP_MOD(I, 4))) (void) { return *(m_data + I); }

			BOOST_PP_REPEAT(16,
				__MYE_MATH_DEFINE_MATRIX4_GET_SET,
				0)

			BOOST_PP_REPEAT(16,
				__MYE_MATH_DEFINE_MATRIX4_REFS,
				0)

		private:

			T m_data[16];

		};

		template <typename T>
		inline Matrix<T, 4, 4> operator* (T s, const Matrix<T, 4, 4> &m);

	}

}

#include "Matrix4.inl"