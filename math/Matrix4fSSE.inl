#pragma once

#ifdef MYE_USE_SSE

#define _00(__m) *((__m))
#define _01(__m) *((__m) + 1)
#define _02(__m) *((__m) + 2)
#define _03(__m) *((__m) + 3)
#define _10(__m) *((__m) + 4)
#define _11(__m) *((__m) + 5)
#define _12(__m) *((__m) + 6)
#define _13(__m) *((__m) + 7)
#define _20(__m) *((__m) + 8)
#define _21(__m) *((__m) + 9)
#define _22(__m) *((__m) + 10)
#define _23(__m) *((__m) + 11)
#define _30(__m) *((__m) + 12)
#define _31(__m) *((__m) + 13)
#define _32(__m) *((__m) + 14)
#define _33(__m) *((__m) + 15)

namespace mye
{

	namespace math
	{

		
		__MYE_MATH_INLINE Matrix<float, 4, 4>::Matrix(void)
		{
		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4>::Matrix(float d)
		{
			m_vector[0] = _mm_set_ps(0, 0, 0, 1);
			m_vector[1] = _mm_set_ps(0, 0, 1, 0);
			m_vector[2] = _mm_set_ps(0, 1, 0, 0);
			m_vector[3] = _mm_set_ps(1, 0, 0, 0);
		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4>::Matrix(const Matrix<float, 3, 3> & m)
		{

			__m128 mask = _mm_castsi128_ps(_mm_set_epi32(0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));
			
			m_vector[0] = _mm_and_ps(mask, m.m_vector[0]);
			m_vector[1] = _mm_and_ps(mask, m.m_vector[1]);
			m_vector[2] = _mm_and_ps(mask, m.m_vector[2]);
			m_vector[3] = _mm_set_ps(1, 0, 0, 0);

		}


		__MYE_MATH_INLINE Matrix<float, 4, 4>::Matrix(std::initializer_list<float> initializer)
		{
			m_vector[0] = _mm_loadu_ps(initializer.begin());
			m_vector[1] = _mm_loadu_ps(initializer.begin() + 4);
			m_vector[2] = _mm_loadu_ps(initializer.begin() + 8);
			m_vector[3] = _mm_loadu_ps(initializer.begin() + 12);
		}


		__MYE_MATH_INLINE void Matrix<float, 4, 4>::Fill(float x)
		{
			m_vector[0] = _mm_set1_ps(x);
			m_vector[1] = _mm_set1_ps(x);
			m_vector[2] = _mm_set1_ps(x);
			m_vector[3] = _mm_set1_ps(x);
		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 1> Matrix<float, 4, 4>::GetRow(int i) const
		{
			
			return Matrix<float, 4, 1>(m_vector[i]);

		}

		
		__MYE_MATH_INLINE void Matrix<float, 4, 4>::SetRow(int i, const Matrix<float, 4, 1> & v)
		{
			assert(i >= 0 && i < 4);
			m_vector[i] = v.m_vector;
		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 1> Matrix<float, 4, 4>::GetColumn(int i) const
		{
			assert(i >= 0 && i < 4);
			return Matrix<float, 4, 1>(
				__MYE_MATRIX_ACCESS(m_data, 4, 0, i),
				__MYE_MATRIX_ACCESS(m_data, 4, 1, i),
				__MYE_MATRIX_ACCESS(m_data, 4, 2, i),
				__MYE_MATRIX_ACCESS(m_data, 4, 3, i));
		}

		
		__MYE_MATH_INLINE void Matrix<float, 4, 4>::SetColumn(int i, const Matrix<float, 4, 1> & v)
		{
			assert(i >= 0 && i < 4);
			__MYE_MATRIX_ACCESS(m_data, 4, 0, i) = v[0];
			__MYE_MATRIX_ACCESS(m_data, 4, 1, i) = v[1];
			__MYE_MATRIX_ACCESS(m_data, 4, 2, i) = v[2];
			__MYE_MATRIX_ACCESS(m_data, 4, 3, i) = v[3];
		}

		
		__MYE_MATH_INLINE float & Matrix<float, 4, 4>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 4 && j >= 0 && j < 4);
			return __MYE_MATRIX_ACCESS(m_data, 4, i, j);
		}

		
		__MYE_MATH_INLINE const float& Matrix<float, 4, 4>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 4 && j >= 0 && j < 4);
			return __MYE_MATRIX_ACCESS(m_data, 4, i, j);
		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> Matrix<float, 4, 4>::operator* (float s) const
		{

			Matrix<float, 4, 4> r;

			r.m_vector[0] = _mm_mul_ps(m_vector[0], _mm_set1_ps(s));
			r.m_vector[1] = _mm_mul_ps(m_vector[1], _mm_set1_ps(s));
			r.m_vector[2] = _mm_mul_ps(m_vector[2], _mm_set1_ps(s));
			r.m_vector[3] = _mm_mul_ps(m_vector[3], _mm_set1_ps(s));

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> Matrix<float, 4, 4>::operator/ (float s) const
		{
			return *this * (1.0f / s);
		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> & Matrix<float, 4, 4>::operator= (const Matrix<float, 4, 4> & m)
		{
			m_vector[0] = m.m_vector[0];
			m_vector[1] = m.m_vector[1];
			m_vector[2] = m.m_vector[2];
			m_vector[3] = m.m_vector[3];
			return *this;
		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> Matrix<float, 4, 4>::operator* (const Matrix<float, 4, 4> & b) const
		{

			Matrix<float, 4, 4> t = b.Transpose();
			Matrix<float, 4, 4> r;

			{
				__m128 t0, t1, t2;

				t0 = _mm_or_ps(_mm_dp_ps(m_vector[0], t.m_vector[0], 0xF1),
							   _mm_dp_ps(m_vector[0], t.m_vector[1], 0xF2));

				t1 = _mm_or_ps(_mm_dp_ps(m_vector[0], t.m_vector[2], 0xF4),
							   _mm_dp_ps(m_vector[0], t.m_vector[3], 0xF8));

				t2 = _mm_or_ps(t0, t1);

				r.m_vector[0] = t2;
			}


			{
				__m128 t0, t1, t2;

				t0 = _mm_or_ps(_mm_dp_ps(m_vector[1], t.m_vector[0], 0xF1),
							   _mm_dp_ps(m_vector[1], t.m_vector[1], 0xF2));

				t1 = _mm_or_ps(_mm_dp_ps(m_vector[1], t.m_vector[2], 0xF4),
							   _mm_dp_ps(m_vector[1], t.m_vector[3], 0xF8));

				t2 = _mm_or_ps(t0, t1);

				r.m_vector[1] = t2;
			}

			{
				__m128 t0, t1, t2;

				t0 = _mm_or_ps(_mm_dp_ps(m_vector[2], t.m_vector[0], 0xF1),
							   _mm_dp_ps(m_vector[2], t.m_vector[1], 0xF2));

				t1 = _mm_or_ps(_mm_dp_ps(m_vector[2], t.m_vector[2], 0xF4),
							   _mm_dp_ps(m_vector[2], t.m_vector[3], 0xF8));

				t2 = _mm_or_ps(t0, t1);

				r.m_vector[2] = t2;
			}

			{
				__m128 t0, t1, t2;

				t0 = _mm_or_ps(_mm_dp_ps(m_vector[3], t.m_vector[0], 0xF1),
							   _mm_dp_ps(m_vector[3], t.m_vector[1], 0xF2));

				t1 = _mm_or_ps(_mm_dp_ps(m_vector[3], t.m_vector[2], 0xF4),
							   _mm_dp_ps(m_vector[3], t.m_vector[3], 0xF8));

				t2 = _mm_or_ps(t0, t1);

				r.m_vector[3] = t2;
			}

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> Matrix<float, 4, 4>::operator+ (const Matrix<float, 4, 4> & b) const
		{

			Matrix<float, 4, 4> r;

			r.m_vector[0] = _mm_add_ps(m_vector[0], b.m_vector[0]);
			r.m_vector[1] = _mm_add_ps(m_vector[1], b.m_vector[1]);
			r.m_vector[2] = _mm_add_ps(m_vector[2], b.m_vector[2]);
			r.m_vector[3] = _mm_add_ps(m_vector[3], b.m_vector[3]);

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> Matrix<float, 4, 4>::operator- (const Matrix<float, 4, 4> & b) const
		{

			Matrix<float, 4, 4> r;

			r.m_vector[0] = _mm_sub_ps(m_vector[0], b.m_vector[0]);
			r.m_vector[1] = _mm_sub_ps(m_vector[1], b.m_vector[1]);
			r.m_vector[2] = _mm_sub_ps(m_vector[2], b.m_vector[2]);
			r.m_vector[3] = _mm_sub_ps(m_vector[3], b.m_vector[3]);

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> Matrix<float, 4, 4>::Transpose(void) const
		{

			Matrix<float, 4, 4> r;

			__m128 t3, t2, t1, t0;
			
			t0   = _mm_shuffle_ps(m_vector[0], m_vector[1], 0x44);
			t2   = _mm_shuffle_ps(m_vector[0], m_vector[1], 0xEE);
			t1   = _mm_shuffle_ps(m_vector[2], m_vector[3], 0x44);
			t3   = _mm_shuffle_ps(m_vector[2], m_vector[3], 0xEE);
			
			r.m_vector[0] = _mm_shuffle_ps(t0, t1, 0x88);    
			r.m_vector[1] = _mm_shuffle_ps(t0, t1, 0xDD);    
			r.m_vector[3] = _mm_shuffle_ps(t2, t3, 0xDD);  
			r.m_vector[2] = _mm_shuffle_ps(t2, t3, 0x88);    
			
			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> Matrix<float, 4, 4>::operator- (void) const
		{

			Matrix<float, 4, 4> r;

			r.m_vector[0] = _mm_xor_ps(m_vector[0], _mm_set1_ps(-0.0f));
			r.m_vector[1] = _mm_xor_ps(m_vector[1], _mm_set1_ps(-0.0f));
			r.m_vector[2] = _mm_xor_ps(m_vector[2], _mm_set1_ps(-0.0f));
			r.m_vector[3] = _mm_xor_ps(m_vector[3], _mm_set1_ps(-0.0f));

			return r;

		}

		
		__MYE_MATH_INLINE Matrix<float, 4, 4> Matrix<float, 4, 4>::Inverse(void) const
		{

			Matrix<float, 4, 4> r;
			detail::SSEInverse4(m_vector, r.m_vector);
			return r;

		}

		
		__MYE_MATH_INLINE float Matrix<float, 4, 4>::Determinant(void) const
		{
			return _mm_cvtss_f32(detail::SSEDeterminant4(m_vector));
		}

		__MYE_MATH_INLINE float * Matrix<float, 4, 4>::Data(void)
		{
			return m_data;
		}

		__MYE_MATH_INLINE const float * Matrix<float, 4, 4>::Data(void) const
		{
			return m_data;
		}

	}

}

#undef _00
#undef _01
#undef _02
#undef _03
#undef _10
#undef _11
#undef _12
#undef _13
#undef _20
#undef _21
#undef _22
#undef _23
#undef _30
#undef _31
#undef _32
#undef _33

#endif