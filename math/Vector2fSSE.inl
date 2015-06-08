#ifdef MYE_USE_SSE

namespace mye
{

	namespace math
	{

		
		__MYE_MATH_INLINE Matrix<float, 2, 1>::Matrix(void)
		{
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1>::Matrix(float v)
		{
			m_vector = _mm_set1_ps(v);
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1>::Matrix(float x, float y)
		{
			m_vector = _mm_set_ps(1.0f, 0.0f, y, x);
		}


		__MYE_MATH_INLINE Matrix<float, 2, 1>::Matrix(__m128 v)
		{
			m_vector = v;
		}

		
		__MYE_MATH_INLINE float & Matrix<float, 2, 1>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 2 && j == 0);
			return m_data[i];
		}

		
		__MYE_MATH_INLINE const float & Matrix<float, 2, 1>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 2 && j == 0);
			return m_data[i];
		}

		
		__MYE_MATH_INLINE float & Matrix<float, 2, 1>::operator[] (int i)
		{
			assert(i >= 0 && i < 2);
			return m_data[i];
		}

		
		__MYE_MATH_INLINE const float & Matrix<float, 2, 1>::operator[] (int i) const
		{
			assert(i >= 0 && i < 2);
			return m_data[i];
		}

		
		__MYE_MATH_INLINE float Matrix<float, 2, 1>::Dot(const Matrix<float, 2, 1> & v) const
		{
			return _mm_cvtss_f32(detail::SSEDot2(m_vector, v.m_vector));
		}

		
		__MYE_MATH_INLINE bool Matrix<float, 2, 1>::operator== (const Matrix<float, 2, 1> & b) const
		{
			__m128 t0 = _mm_set1_ps(0.0f);
			__m128 t1 = _mm_movelh_ps(m_vector, t0);
			__m128 t2 = _mm_movelh_ps(b.m_vector, t0);
			__m128 t3 = _mm_cmpeq_ps(t1, t2);
			return 0 != _mm_movemask_ps(t3);
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::operator+ (const Matrix<float, 2, 1> & b) const
		{
			return Matrix<float, 2, 1>(_mm_add_ps(m_vector, b.m_vector));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::operator- (const Matrix<float, 2, 1> & b) const
		{
			return Matrix<float, 2, 1>(_mm_sub_ps(m_vector, b.m_vector));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::operator* (const Matrix<float, 2, 1> & b) const
		{
			return Matrix<float, 2, 1>(_mm_mul_ps(m_vector, b.m_vector));
		}

		
		__MYE_MATH_INLINE  Matrix<float, 2, 1> Matrix<float, 2, 1>::operator/ (const Matrix<float, 2, 1> & b) const
		{
			return Matrix<float, 2, 1>(_mm_div_ps(m_vector, b.m_vector));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::operator* (float x) const
		{
			__m128 t0 = _mm_set1_ps(x);
			return Matrix<float, 2, 1>(_mm_mul_ps(m_vector, t0));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::operator/ (float x) const
		{
			__m128 t0 = detail::SSENewtonRaphson<1>::ReciprocalSS(_mm_set_ss(x));
			__m128 t1 = detail::SplatSS(t0);

			return Matrix<float, 2, 1>(_mm_mul_ps(m_vector, t1));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::operator- (void) const
		{
			__m128 t0 = _mm_set1_ps(-0.0f);
			__m128 t1 = _mm_xor_ps(m_vector, t0);
			return Matrix<float, 2, 1>(t1);
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> & Matrix<float, 2, 1>::operator= (const Matrix<float, 2, 1> & b)
		{
			m_vector = b.m_vector;
			return *this;
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::Normalize(void) const
		{
			__m128 t0 = detail::SSEDot2(m_vector, m_vector);
			__m128 t1 = detail::SSENewtonRaphson<1>::ReciprocalSquareRootSS(t0);
			__m128 t2 = detail::SplatSS(t1);
			__m128 t3 = _mm_mul_ps(t2, m_vector);

			return Matrix<float, 2, 1>(t3);
		}


		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::NormalizeFast(void) const
		{
			__m128 t0 = detail::SplatSS(detail::SSEDot2(m_vector, m_vector));
			__m128 t1 = _mm_rsqrt_ps(t0);
			__m128 t2 = detail::SplatSS(t1);
			__m128 t3 = _mm_mul_ps(t2, m_vector);

			return Matrix<float, 2, 1>(t3);
		}



		__MYE_MATH_INLINE float Matrix<float, 2, 1>::Length(void) const
		{
			return _mm_cvtss_f32(_mm_sqrt_ss(detail::SSEDot2(m_vector, m_vector)));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::CwiseAbs(void) const
		{
			return Matrix<float, 2, 1>(_mm_and_ps(m_vector, _mm_castsi128_ps(_mm_set_epi32(0x0, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF))));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::CwiseMin(const Matrix<float, 2, 1> & v) const
		{
			return Matrix<float, 2, 1>(_mm_min_ps(m_vector, v.m_vector));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::CwiseMax(const Matrix<float, 2, 1> & v) const
		{
			return Matrix<float, 2, 1>(_mm_max_ps(m_vector, v.m_vector));
		}

		
		__MYE_MATH_INLINE float * Matrix<float, 2, 1>::Data(void)
		{
			return m_data;
		}

		
		__MYE_MATH_INLINE const float * Matrix<float, 2, 1>::Data(void) const
		{
			return m_data;
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::Clamp(float minimum, float maximum)
		{
			return Clamp(Matrix<float, 2, 1>(minimum), Matrix<float, 2, 1>(maximum));
		}

		
		__MYE_MATH_INLINE Matrix<float, 2, 1> Matrix<float, 2, 1>::Clamp(const Matrix<float, 2, 1> & minimum, const Matrix<float, 2, 1> & maximum)
		{

			return this->CwiseMax(Matrix<float, 2, 1>(minimum)).CwiseMin(Matrix<float, 2, 1>(maximum));

		}

#include "SwizzleVector2f.inl"

	}

}

#endif