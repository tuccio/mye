namespace mye
{

	namespace math
	{

		
		__MYE_MATH_INLINE QuaternionTempl<float>::QuaternionTempl(void)
		{
		}

		
		__MYE_MATH_INLINE QuaternionTempl<float>::QuaternionTempl(float w, float x, float y, float z)
		{
			m_vector = _mm_set_ps(w, z, y, x);
		}

		
		__MYE_MATH_INLINE QuaternionTempl<float>::QuaternionTempl(const Matrix<float, 3, 1> & axis, float angle)
		{

			float angleRadians = Radians(angle);

			float sinHalfAngle = Sine(angleRadians * 0.5f);
			float cosHalfAngle = Cosine(angleRadians * 0.5f);

			__m128 mask = _mm_castsi128_ps(_mm_set_epi32(0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));;
			__m128 t0 = _mm_mul_ps(axis.m_vector, _mm_set1_ps(sinHalfAngle));
			__m128 t1 = _mm_and_ps(t0, mask);
			__m128 t2 = _mm_set_ps(cosHalfAngle, 0.0f, 0.0f, 0.0f);

			m_vector = _mm_or_ps(t1, t2);

		}

		
		__MYE_MATH_INLINE QuaternionTempl<float>::QuaternionTempl(const Matrix<float, 3, 3> & A)
		{	

			m_data[3] = Sqrt(1 + A.m00() + A.m11() + A.m22()) * 0.5f;

			float invFourW = 1.0f / (4.0f * m_data[3]);

			m_data[0] = (A.m21() - A.m12()) * invFourW;
			m_data[1] = (A.m02() - A.m20()) * invFourW;
			m_data[2] = (A.m10() - A.m01()) * invFourW;

		}


		__MYE_MATH_INLINE QuaternionTempl<float>::QuaternionTempl(__m128 v) :
			m_vector(v)
		{
		}

		
		__MYE_MATH_INLINE QuaternionTempl<float> QuaternionTempl<float>::Inverse(void) const
		{

			__m128 t0 = detail::SSEDot4(m_vector, m_vector);
			__m128 t1 = detail::SSENewtonRaphson<1>::ReciprocalSS(t0);
			__m128 t2 = detail::SplatSS(t1);
			__m128 t3 = _mm_mul_ps(t0, t2);

			return QuaternionTempl<float>(t3);

		}

		
		__MYE_MATH_INLINE QuaternionTempl<float> QuaternionTempl<float>::Conjugate(void) const
		{

			__m128 mask = _mm_set_ps(0.0f, -0.0f, -0.0f, -0.0f);
			__m128 t0   = _mm_xor_ps(mask, m_vector);

			return QuaternionTempl<float>(t0);

		}

		
		__MYE_MATH_INLINE Matrix<float, 3, 1> QuaternionTempl<float>::Rotate(const Matrix<float, 3, 1> & p) const
		{

			__m128 t0 = _mm_castsi128_ps(_mm_set_epi32(0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));
			__m128 t1 = _mm_and_ps(p.m_vector, t0);

			QuaternionTempl<float> pQuat(t1);
			QuaternionTempl<float> r = *this * pQuat * Conjugate();

			return Matrix<float, 3, 1>(r.m_vector);

		}

		
		__MYE_MATH_INLINE float QuaternionTempl<float>::Norm(void) const
		{
			__m128 t0 = detail::SSEDot4(m_vector, m_vector);
			__m128 t1 = detail::SSENewtonRaphson<1>::SquareRootSS(t0);
			return _mm_cvtss_f32(t1);
		}

		
		__MYE_MATH_INLINE QuaternionTempl<float> QuaternionTempl<float>::Normalize(void) const
		{

			__m128 t0 = detail::SSEDot4(m_vector, m_vector);
			__m128 t1 = detail::SSENewtonRaphson<1>::ReciprocalSquareRootSS(t0);
			__m128 t2 = _mm_mul_ps(m_vector, detail::SplatSS(t1));

			return QuaternionTempl<float>(t2);

		}

		
		__MYE_MATH_INLINE QuaternionTempl<float> QuaternionTempl<float>::operator+ (const QuaternionTempl & q) const
		{
			return QuaternionTempl<float>(_mm_and_ps(q.m_vector, m_vector));
		}

		
		__MYE_MATH_INLINE QuaternionTempl<float> QuaternionTempl<float>::operator- (const QuaternionTempl & q) const
		{
			return QuaternionTempl<float>(_mm_sub_ps(q.m_vector, m_vector));
		}

		
		__MYE_MATH_INLINE QuaternionTempl<float> QuaternionTempl<float>::operator* (const QuaternionTempl & q) const
		{

			// (z0, y0, x0, w0)
			__m128 t1 = _mm_shuffle_ps(m_vector, m_vector, _MM_SHUFFLE(3, 0, 1, 2));

			// (y1, z1, w1, x1)
			__m128 t2 = _mm_shuffle_ps(q.m_vector, q.m_vector, _MM_SHUFFLE(0, 3, 2, 1));

			// (x0, z0, y0, w0)
			__m128 t3 = _mm_shuffle_ps(m_vector, m_vector, _MM_SHUFFLE(3, 1, 2, 0));

			// (z1, x1, w1, y1)
			__m128 t4 = _mm_shuffle_ps(q.m_vector, q.m_vector, _MM_SHUFFLE(1, 3, 0, 2));

			// (y0, x0, z0, w0)
			__m128 t5 = _mm_shuffle_ps(m_vector, m_vector, _MM_SHUFFLE(3, 2, 0, 1));

			// (x1, y1, w1, z1)
			__m128 t6 = _mm_shuffle_ps(q.m_vector, q.m_vector, _MM_SHUFFLE(2, 3, 1, 0));

			// (x0x1, y0y1, z0z1, w0w1)
			__m128 q0 = _mm_mul_ps(m_vector, q.m_vector);

			// (z0y1, y0z1, x0w1, w0x1)
			__m128 q1 = _mm_mul_ps(t1, t2);

			// (x0z1, z0x1, y0w1, w0y1)
			__m128 q2 = _mm_mul_ps(t3, t4);

			// (y0x1, x0y1, z0w1, w0z1)
			__m128 q3 = _mm_mul_ps(t5, t6);

			__m128 mask0 = _mm_set_ps(0.0f, -0.0f, -0.0f, -0.0f);
			__m128 mask1 = _mm_set_ps(0.0f, 0.0f, 0.0f, -0.0f);

			// (-x0x1, -y0y1, -z0z1, w0w1)
			q0 = _mm_xor_ps(mask0, q0);
			// (-z0y1, y0z1, x0w1, w0x1)
			q1 = _mm_xor_ps(mask1, q1);
			// (-x0z1, z0x1, y0w1, w0y1)
			q2 = _mm_xor_ps(mask1, q2);
			// (-y0x1, x0y1, z0w1, w0z1)
			q3 = _mm_xor_ps(mask1, q3);

			// (qx, qx, qy, qy)
			__m128 t7 = detail::SumComponents(q1, q2);

			// (qz, qz, qw, qw)
			__m128 t8 = detail::SumComponents(q3, q0);

			// (qx, qy, qz, qw)
			__m128 t9 = _mm_shuffle_ps(t7, t8, _MM_SHUFFLE(2, 0, 2, 0));

			return QuaternionTempl<float>(t9);
		}

		
		__MYE_MATH_INLINE float & QuaternionTempl<float>::operator[] (int i)
		{
			return m_data[i];
		}

		
		__MYE_MATH_INLINE const float & QuaternionTempl<float>::operator[] (int i) const
		{
			return m_data[i];
		}

		
		__MYE_MATH_INLINE float & QuaternionTempl<float>::x(void)
		{
			return m_data[0];
		}

		
		__MYE_MATH_INLINE const float & QuaternionTempl<float>::x(void) const
		{
			return m_data[0];
		}

		
		__MYE_MATH_INLINE float & QuaternionTempl<float>::y(void)
		{
			return m_data[1];
		}

		
		__MYE_MATH_INLINE const float & QuaternionTempl<float>::y(void) const
		{
			return m_data[1];
		}

		
		__MYE_MATH_INLINE float & QuaternionTempl<float>::z(void)
		{
			return m_data[2];
		}

		
		__MYE_MATH_INLINE const float & QuaternionTempl<float>::z(void) const
		{
			return m_data[2];
		}

		
		__MYE_MATH_INLINE float & QuaternionTempl<float>::w(void)
		{
			return m_data[3];
		}

		
		__MYE_MATH_INLINE const float & QuaternionTempl<float>::w(void) const
		{
			return m_data[3];
		}

	}

}