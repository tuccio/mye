#ifdef MYE_USE_SSE

namespace mye
{

	namespace math
	{

		__MYE_MATH_INLINE Matrix<float, 3, 1>::Matrix(void)
		{
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1>::Matrix(float v)
		{
			m_vector = _mm_set1_ps(v);
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1>::Matrix(float x, float y, float z)
		{
			m_vector = _mm_set_ps(0, z, y, x);
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1>::Matrix(const Matrix<float, 2, 1> & v, float z)
		{
			__m128 t0 = _mm_set1_ps(z);
			m_vector = _mm_movelh_ps(v.m_vector, t0);
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1>::Matrix(__m128 v)
		{
			m_vector = v;
		}

		__MYE_MATH_INLINE float & Matrix<float, 3, 1>::operator() (int i, int j)
		{
			assert(i >= 0 && i < 3 && j == 0);
			return m_data[i];
		}

		__MYE_MATH_INLINE const float & Matrix<float, 3, 1>::operator() (int i, int j) const
		{
			assert(i >= 0 && i < 3 && j == 0);
			return m_data[i];
		}

		__MYE_MATH_INLINE float & Matrix<float, 3, 1>::operator[] (int i)
		{
			assert(i >= 0 && i < 3);
			return m_data[i];
		}

		__MYE_MATH_INLINE const float & Matrix<float, 3, 1>::operator[] (int i) const
		{
			assert(i >= 0 && i < 3);
			return m_data[i];
		}

		__MYE_MATH_INLINE float Matrix<float, 3, 1>::Dot(const Matrix<float, 3, 1> & v) const
		{
			return _mm_cvtss_f32(detail::SSEDot3(m_vector, v.m_vector));
		}

		__MYE_MATH_INLINE bool Matrix<float, 3, 1>::operator== (const Matrix<float, 3, 1> & b) const
		{
			__m128 t0 = _mm_cmpeq_ps(m_vector, b.m_vector);
			return _mm_movemask_ps(t0) == 0x7;
		}

		__MYE_MATH_INLINE bool Matrix<float, 3, 1>::operator!= (const Matrix<float, 3, 1> & b) const
		{
			return !(*this == b);
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::Cross(const Matrix<float, 3, 1> & v) const
		{

			// t0 = (y0, z0, x0)
			__m128 t0 = _mm_shuffle_ps(m_vector, m_vector, _MM_SHUFFLE(3, 0, 2, 1));

			// t1 = (z1, x1, y1)
			__m128 t1 = _mm_shuffle_ps(v.m_vector, v.m_vector, _MM_SHUFFLE(3, 1, 0, 2));

			// t2 = (z0, x0, y1)
			__m128 t2 = _mm_shuffle_ps(m_vector, m_vector, _MM_SHUFFLE(3, 1, 0, 2));

			// t3 = (y0, z0, x0)
			__m128 t3 = _mm_shuffle_ps(v.m_vector, v.m_vector, _MM_SHUFFLE(3, 0, 2, 1));

			return Matrix<float, 3, 1>(_mm_sub_ps(_mm_mul_ps(t0, t1), _mm_mul_ps(t2, t3)));

		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::operator+ (const Matrix<float, 3, 1> & b) const
		{
			return Matrix<float, 3, 1>(_mm_add_ps(m_vector, b.m_vector));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::operator- (const Matrix<float, 3, 1> & b) const
		{
			return Matrix<float, 3, 1>(_mm_sub_ps(m_vector, b.m_vector));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::operator* (const Matrix<float, 3, 1> & b) const
		{
			return Matrix<float, 3, 1>(_mm_mul_ps(m_vector, b.m_vector));
		}

		__MYE_MATH_INLINE  Matrix<float, 3, 1> Matrix<float, 3, 1>::operator/ (const Matrix<float, 3, 1> & b) const
		{
			return Matrix<float, 3, 1>(_mm_div_ps(m_vector, b.m_vector));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::operator* (float x) const
		{
			return *this * Matrix<float, 3, 1>(x);
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::operator/ (float x) const
		{
			__m128 t0 = detail::SSENewtonRaphson<1>::ReciprocalSS(_mm_set_ss(x));
			__m128 t1 = detail::SplatSS(t0);

			return Matrix<float, 3, 1>(_mm_mul_ps(m_vector, t1));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::operator- (void) const
		{
			return Matrix<float, 3, 1>(_mm_xor_ps(m_vector, _mm_set1_ps(-0.0f)));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> & Matrix<float, 3, 1>::operator= (const Matrix<float, 3, 1> & v)
		{
			m_vector = v.m_vector;
			return *this;
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::Normalize(void) const
		{

			__m128 t0 = detail::SSEDot3(m_vector, m_vector);
			__m128 t1 = detail::SSENewtonRaphson<1>::ReciprocalSquareRootSS(t0);
			__m128 t2 = detail::SplatSS(t1);
			__m128 t3 = _mm_mul_ps(t2, m_vector);

			return Matrix<float, 3, 1>(t3);

		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::NormalizeFast(void) const
		{

			__m128 t0 = detail::SplatSS(detail::SSEDot3(m_vector, m_vector));
			__m128 t1 = _mm_rsqrt_ps(t0);
			__m128 t2 = _mm_mul_ps(t1, m_vector);

			return Matrix<float, 3, 1>(t2);

		}

		__MYE_MATH_INLINE float Matrix<float, 3, 1>::Length(void) const
		{
			return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(m_vector, m_vector, 0x71)));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::Reflect(const Matrix<float, 3, 1> & normal) const
		{
			return *this - (2.0f * this->Dot(normal)) * normal;
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::CwiseAbs(void) const
		{
			return Matrix<float, 3, 1>(_mm_and_ps(m_vector, _mm_castsi128_ps(_mm_set_epi32(0x0, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF))));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::CwiseMin(const Matrix<float, 3, 1> & v) const
		{
			return Matrix<float, 3, 1>(_mm_min_ps(m_vector, v.m_vector));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::CwiseMax(const Matrix<float, 3, 1> & v) const
		{
			return Matrix<float, 3, 1>(_mm_max_ps(m_vector, v.m_vector));
		}

		__MYE_MATH_INLINE float * Matrix<float, 3, 1>::Data(void)
		{
			return m_data;
		}

		__MYE_MATH_INLINE const float * Matrix<float, 3, 1>::Data(void) const
		{
			return m_data;
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::Clamp(float minimum, float maximum)
		{
			return Clamp(Matrix<float, 3, 1>(minimum), Matrix<float, 3, 1>(maximum));
		}

		__MYE_MATH_INLINE Matrix<float, 3, 1> Matrix<float, 3, 1>::Clamp(const Matrix<float, 3, 1> & minimum, const Matrix<float, 3, 1> & maximum)
		{
			return this->CwiseMax(Matrix<float, 3, 1>(minimum)).CwiseMin(Matrix<float, 3, 1>(maximum));
		}

#include "SwizzleVector3f.inl"

	}

}


#endif