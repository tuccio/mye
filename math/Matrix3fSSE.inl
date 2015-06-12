#pragma once

#ifdef MYE_USE_SSE

#define _00(__m) *((__m))
#define _01(__m) *((__m) + 1)
#define _02(__m) *((__m) + 2)
#define _10(__m) *((__m) + 4)
#define _11(__m) *((__m) + 5)
#define _12(__m) *((__m) + 6)
#define _20(__m) *((__m) + 8)
#define _21(__m) *((__m) + 9)
#define _22(__m) *((__m) + 10)

namespace mye
{

	namespace math
	{

		
		__MYE_MATH_INLINE Matrix<float, 3, 3>::Matrix(void)
		{
		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3>::Matrix(const Matrix<float, 4, 4> & m)
		{
			m_vector[0] = m.m_vector[0];
			m_vector[1] = m.m_vector[1];
			m_vector[2] = m.m_vector[2];
		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3>::Matrix(float d)
		{
			m_vector[0] = _mm_set_ps(0, 0, 0, d);
			m_vector[1] = _mm_set_ps(0, 0, d, 0);
			m_vector[2] = _mm_set_ps(0, d, 0, 0);
			
		}

		
		__MYE_MATH_INLINE void Matrix<float, 3, 3>::Fill(float x)
		{
			m_vector[0] = m_vector[1] = m_vector[2] = _mm_set1_ps(x);
		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 3>::GetRow(int i) const
		{
			assert(i >= 0 && i < 3);
			return Matrix<float, 3, 1>(m_vector[i]);
		}

		
		__MYE_MATH_INLINE void Matrix<float, 3, 3>::SetRow(int i, const Matrix<float, 3, 1> & v)
		{
			assert(i >= 0 && i < 3);
			m_vector[i] = v.m_vector;
		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 3>::GetColumn(int j) const
		{
			assert(j >= 0 && j < 3);
			return Matrix<float, 3, 1>(__MYE_MATRIX_ACCESS(m_data, 4, 0, j),
			                           __MYE_MATRIX_ACCESS(m_data, 4, 1, j),
			                           __MYE_MATRIX_ACCESS(m_data, 4, 2, j));
		}

		
		__MYE_MATH_INLINE void Matrix<float, 3, 3>::SetColumn(int j, const Matrix<float, 3, 1> &v)
		{
			assert(j >= 0 && j < 3);
			__MYE_MATRIX_ACCESS(m_data, 4, 0, j) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 4, 1, j) = v[1];
			__MYE_MATRIX_ACCESS(m_data, 4, 2, j) = v[2];
		}

		
		__MYE_MATH_INLINE float & Matrix<float, 3, 3>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 3 && j >= 0 && j < 3);
			return __MYE_MATRIX_ACCESS(m_data, 4, i, j);
		}

		
		__MYE_MATH_INLINE const float & Matrix<float, 3, 3>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 3 && j >= 0 && j < 3);
			return __MYE_MATRIX_ACCESS(m_data, 4, i, j);
		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3> Matrix<float, 3, 3>::operator- (void) const
		{

			Matrix<float, 3, 3> r;

			r.m_vector[0] = _mm_xor_ps(m_vector[0], _mm_set1_ps(-0.0f));
			r.m_vector[1] = _mm_xor_ps(m_vector[1], _mm_set1_ps(-0.0f));
			r.m_vector[2] = _mm_xor_ps(m_vector[2], _mm_set1_ps(-0.0f));

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3> & Matrix<float, 3, 3>::operator= (const Matrix<float, 3, 3> & a)
		{

			m_vector[0] = a.m_vector[0];
			m_vector[1] = a.m_vector[1];
			m_vector[2] = a.m_vector[2];

			return *this;

		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3> Matrix<float, 3, 3>::operator+ (const Matrix<float, 3, 3> & a) const
		{

			Matrix<float, 3, 3> r;

			r.m_vector[0] =_mm_add_ps(m_vector[0], a.m_vector[0]);
			r.m_vector[1] =_mm_add_ps(m_vector[1], a.m_vector[1]);
			r.m_vector[2] =_mm_add_ps(m_vector[2], a.m_vector[2]);

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3> Matrix<float, 3, 3>::operator- (const Matrix<float, 3, 3> & a) const
		{

			Matrix<float, 3, 3> r;

			r.m_vector[0] =_mm_sub_ps(m_vector[0], a.m_vector[0]);
			r.m_vector[1] =_mm_sub_ps(m_vector[1], a.m_vector[1]);
			r.m_vector[2] =_mm_sub_ps(m_vector[2], a.m_vector[2]);

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3> Matrix<float, 3, 3>::operator* (const Matrix<float, 3, 3> & b) const
		{

			Matrix<float, 3, 3> t = b.Transpose();

			Matrix<float, 3, 3> r;

			{
				__m128 t0, t1, t2;

				t0 = _mm_or_ps(_mm_dp_ps(m_vector[0], t.m_vector[0], 0x71),
							   _mm_dp_ps(m_vector[0], t.m_vector[1], 0x72));

				t1 = _mm_dp_ps(m_vector[0], t.m_vector[2], 0x74);

				t2 = _mm_or_ps(t0, t1);

				r.m_vector[0] = t2;
			}

			{
				__m128 t0, t1, t2;

				t0 = _mm_or_ps(_mm_dp_ps(m_vector[1], t.m_vector[0], 0x71),
							   _mm_dp_ps(m_vector[1], t.m_vector[1], 0x72));

				t1 = _mm_dp_ps(m_vector[1], t.m_vector[2], 0x74);

				t2 = _mm_or_ps(t0, t1);

				r.m_vector[1] = t2;
			}

			{
				__m128 t0, t1, t2;

				t0 = _mm_or_ps(_mm_dp_ps(m_vector[2], t.m_vector[0], 0x71),
							   _mm_dp_ps(m_vector[2], t.m_vector[1], 0x72));

				t1 = _mm_dp_ps(m_vector[2], t.m_vector[2], 0x74);

				t2 = _mm_or_ps(t0, t1);

				r.m_vector[2] = t2;
			}

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3> Matrix<float, 3, 3>::operator* (float s) const
		{

			Matrix<float, 3, 3> r;

			__m128 t0 = _mm_set1_ps(s);

			r.m_vector[0] = _mm_mul_ps(t0, m_vector[0]);
			r.m_vector[1] = _mm_mul_ps(t0, m_vector[1]);
			r.m_vector[2] = _mm_mul_ps(t0, m_vector[2]);

			return r;

		}

		__MYE_MATH_INLINE Matrix<float, 3, 3> Matrix<float, 3, 3>::operator/ (float s) const
		{
			return *this * (1.0f / s);
		}

		
		__MYE_MATH_INLINE bool Matrix<float, 3, 3>::operator== (const Matrix<float, 3, 3> & b) const
		{

			__m128 t0 = _mm_cmpeq_ps(m_vector[0], b.m_vector[0]);
			__m128 t1 = _mm_cmpeq_ps(m_vector[1], b.m_vector[1]);
			__m128 t2 = _mm_cmpeq_ps(m_vector[2], b.m_vector[2]);
			__m128 t3 = _mm_cmpeq_ps(m_vector[3], b.m_vector[3]);

			__m128 t4 = _mm_and_ps(t0, t1);
			__m128 t5 = _mm_and_ps(t2, t3);

			__m128 t6 = _mm_and_ps(t4, t5);

			__m128 mask = _mm_castsi128_ps(_mm_set_epi32(0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));
			__m128 t7 = _mm_and_ps(mask, t6);

			return _mm_movemask_ps(t7) == 0xE;

		}


		__MYE_MATH_INLINE Matrix<float, 3, 3> Matrix<float, 3, 3>::Transpose(void) const
		{

			Matrix<float, 3, 3> r;

			__m128 t0 = _mm_shuffle_ps(m_vector[0], m_vector[1], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 t1 = _mm_shuffle_ps(m_vector[0], m_vector[1], _MM_SHUFFLE(3, 2, 3, 2));
			
			r.m_vector[0] = _mm_shuffle_ps(t0, m_vector[2], _MM_SHUFFLE(0, 0, 2, 0));
			r.m_vector[1] = _mm_shuffle_ps(t0, m_vector[2], _MM_SHUFFLE(0, 1, 3, 1));
			r.m_vector[2] = _mm_shuffle_ps(t1, m_vector[2], _MM_SHUFFLE(0, 2, 2, 0));

			return r;

		}

		
		__MYE_MATH_INLINE float Matrix<float, 3, 3>::Determinant(void) const
		{
			return _mm_cvtss_f32(detail::SSEDeterminant3(m_vector));
		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 3> Matrix<float, 3, 3>::Inverse(void) const
		{

			Matrix<float, 3, 3> r;

			// Compute the transposed cofactors matrix

			__m128 c00c01, c02c10, c11c12, c20c21, c22;
			
			{
				__m128 t0 = _mm_shuffle_ps(m_vector[1], m_vector[2], _MM_SHUFFLE(2, 1, 2, 1));
				__m128 t1 = _mm_shuffle_ps(m_vector[0], m_vector[2], _MM_SHUFFLE(2, 1, 2, 1));
				c00c01 = detail::SSEDeterminant2(t0, t1);
			}

			{
				__m128 t0 = _mm_shuffle_ps(m_vector[0], m_vector[1], _MM_SHUFFLE(2, 1, 2, 1));
				__m128 t1 = _mm_shuffle_ps(m_vector[1], m_vector[2], _MM_SHUFFLE(2, 0, 2, 0));
				c02c10 = detail::SSEDeterminant2(t0, t1);
			}

			{
				__m128 t0 = _mm_shuffle_ps(m_vector[0], m_vector[2], _MM_SHUFFLE(2, 0, 2, 0));
				__m128 t1 = _mm_shuffle_ps(m_vector[0], m_vector[1], _MM_SHUFFLE(2, 0, 2, 0));
				c11c12 = detail::SSEDeterminant2(t0, t1);
			}

			{
				__m128 t0 = _mm_shuffle_ps(m_vector[1], m_vector[2], _MM_SHUFFLE(1, 0, 1, 0));
				__m128 t1 = _mm_shuffle_ps(m_vector[0], m_vector[2], _MM_SHUFFLE(1, 0, 1, 0));
				c20c21 = detail::SSEDeterminant2(t0, t1);
			}

			{
				__m128 t0 = _mm_shuffle_ps(m_vector[0], m_vector[1], _MM_SHUFFLE(1, 0, 1, 0));
				c22 = detail::SSEDeterminant2(t0);
			}

			// Rows of the transposed cofactor matrix
			__m128 row0, row1, row2;

			{

				// Put all the computed determinants in a matrix
				row0 = _mm_shuffle_ps(c00c01, c02c10, _MM_SHUFFLE(0, 0, 2, 0));
				row1 = _mm_shuffle_ps(c11c12, c02c10, _MM_SHUFFLE(2, 2, 2, 0));
				row1 = _mm_shuffle_ps(row1, row1, _MM_SHUFFLE(0, 1, 0, 2));
				row2 = _mm_shuffle_ps(c20c21, c22, _MM_SHUFFLE(0, 0, 2, 0));

				// Change sign according to the permutation parity
				__m128 mask0 = _mm_set_ps(0.0f,  0.0f, -0.0f,  0.0f);
				__m128 mask1 = _mm_set_ps(0.0f, -0.0f,  0.0f, -0.0f);

				row0 = _mm_xor_ps(row0, mask0);
				row1 = _mm_xor_ps(row1, mask1);
				row2 = _mm_xor_ps(row2, mask0);

			}

			// Compute the reciprocal of the original matrix determinant and multiply the cofactors matrix
			__m128 rd = detail::SSENewtonRaphson<1>::ReciprocalSS(detail::SSEDeterminant3(m_vector));
			rd = _mm_shuffle_ps(rd, rd, _MM_SHUFFLE(0, 0, 0, 0));

			r.m_vector[0] = _mm_mul_ps(rd, row0);
			r.m_vector[1] = _mm_mul_ps(rd, row1);
			r.m_vector[2] = _mm_mul_ps(rd, row2);

			return r;

		}

		
		__MYE_MATH_INLINE float * Matrix<float, 3, 3>::Data(void)
		{
			return m_data;
		}

		
		__MYE_MATH_INLINE const float * Matrix<float, 3, 3>::Data(void) const
		{
			return m_data;
		}

	}

}

#undef _00
#undef _01
#undef _02
#undef _10
#undef _11
#undef _12
#undef _20
#undef _21
#undef _22

#endif