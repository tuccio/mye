#pragma once

#ifdef MYE_USE_SSE

#define _00(__m) *((__m))
#define _01(__m) *((__m) + 1)
#define _10(__m) *((__m) + 2)
#define _11(__m) *((__m) + 3)

namespace mye
{

	namespace math
	{

		
		__MYE_MATH_INLINE Matrix<float, 2, 2>::Matrix(void)
		{
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2>::Matrix(float d)
		{
			_00(m_data) = d;
			_01(m_data) = float(0);
			_10(m_data) = float(0);
			_11(m_data) = d;
		}

		__MYE_MATH_INLINE Matrix<float, 2, 2>::Matrix(__m128 v)
		{
			m_vector = v;
		}
		
		__MYE_MATH_INLINE void Matrix<float, 2, 2>::Fill(float x)
		{
			_00(m_data) = x;
			_01(m_data) = x;
			_10(m_data) = x;
			_11(m_data) = x;
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 2>::GetRow(int i) const
		{

			assert(i >= 0 && i < 2);
			
			__m128 rows[] = {
				m_vector,
				_mm_movehl_ps(m_vector, m_vector)
			};

			return Matrix<float, 2, 1>(rows[i]);

		}

		
		__MYE_MATH_INLINE void Matrix<float, 2, 2>::SetRow(int i, const Matrix<float, 2, 1> & v)
		{
			assert(i >= 0 && i < 2);

			__m128 rows[] = {
				_mm_shuffle_ps(v.m_vector, m_vector, _MM_SHUFFLE(1, 0, 1, 0)),
				_mm_shuffle_ps(m_vector, v.m_vector, _MM_SHUFFLE(1, 0, 1, 0)),
			};

			m_vector = rows[i];

		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 2>::GetColumn(int j) const
		{
			assert(j >= 0 && j < 2);
			return Matrix<float, 2, 1>(__MYE_MATRIX_ACCESS(m_data, 2, 0, j),
			                           __MYE_MATRIX_ACCESS(m_data, 2, 1, j));
		}

		
		__MYE_MATH_INLINE void Matrix<float, 2, 2>::SetColumn(int j, const Matrix<float, 2, 1> &v)
		{
			assert(j >= 0 && j < 2);
			__MYE_MATRIX_ACCESS(m_data, 3, 0, j) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 3, 1, j) = v[1];
			__MYE_MATRIX_ACCESS(m_data, 3, 2, j) = v[2];
		}

		
		__MYE_MATH_INLINE float & Matrix<float, 2, 2>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 2 && j >= 0 && j < 2);
			return __MYE_MATRIX_ACCESS(m_data, 2, i, j);
		}

		
		__MYE_MATH_INLINE const float & Matrix<float, 2, 2>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 2 && j >= 0 && j < 2);
			return __MYE_MATRIX_ACCESS(m_data, 2, i, j);
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> Matrix<float, 2, 2>::operator- (void) const
		{
			__m128 t0 = _mm_xor_ps(m_vector, _mm_set1_ps(-0.0f));
			return Matrix<float, 2, 2>(t0);
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> & Matrix<float, 2, 2>::operator= (const Matrix<float, 2, 2> & a)
		{
			m_vector = a.m_vector;
			return *this;
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> Matrix<float, 2, 2>::operator+ (const Matrix<float, 2, 2> & a) const
		{
			__m128 t0 = _mm_add_ps(m_vector, a.m_vector);
			return Matrix<float, 2, 2>(t0);
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> Matrix<float, 2, 2>::operator- (const Matrix<float, 2, 2> & a) const
		{
			__m128 t0 = _mm_sub_ps(m_vector, a.m_vector);
			return Matrix<float, 2, 2>(t0);
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> Matrix<float, 2, 2>::operator* (const Matrix<float, 2, 2> & b) const
		{

			Matrix<float, 2, 2> t = b.Transpose();

			// TODO

			return t;

		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> Matrix<float, 2, 2>::operator* (float s) const
		{
			__m128 t0 = _mm_mul_ps(m_vector, _mm_set1_ps(s));
			return Matrix<float, 2, 2>(t0);
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> Matrix<float, 2, 2>::operator/ (float s) const
		{

			__m128 t0 = detail::SSENewtonRaphson<1>::ReciprocalSS(_mm_set_ss(s));
			__m128 t1 = detail::SplatSS(t0);

			return Matrix<float, 2, 2>(_mm_mul_ps(m_vector, t1));

		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> Matrix<float, 2, 2>::Transpose(void) const
		{
			__m128 t0 = _mm_shuffle_ps(m_vector, m_vector, _MM_SHUFFLE(3, 1, 2, 0));
			return Matrix<float, 2, 2>(t0);
		}

		
		__MYE_MATH_INLINE float Matrix<float, 2, 2>::Determinant(void) const
		{
			return _mm_cvtss_f32(detail::SSEDeterminant2(m_vector));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 2> Matrix<float, 2, 2>::Inverse(void) const
		{

			__m128 t0 = _mm_shuffle_ps(m_vector, m_vector, _MM_SHUFFLE(0, 2, 1, 3));
			__m128 t1 = _mm_xor_ps(t0, _mm_set_ps(0.0f, -0.0f, -0.0f, 0.0f));

			__m128 t2 = detail::SSENewtonRaphson<1>::ReciprocalSS(detail::SSEDeterminant2(m_vector));
			__m128 t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(0, 0, 0, 0));

			__m128 t4 = _mm_mul_ps(t1, t3);

			return Matrix<float, 2, 2>(t4);

		}

		
		__MYE_MATH_INLINE float * Matrix<float, 2, 2>::Data(void)
		{
			return m_data;
		}

		
		__MYE_MATH_INLINE const float * Matrix<float, 2, 2>::Data(void) const
		{
			return m_data;
		}

	}

}

#undef _00
#undef _01
#undef _10
#undef _11

#endif