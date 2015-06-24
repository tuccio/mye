#include <type_traits>

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <int N>
			struct SSENewtonRaphson
			{

				static __MYE_MATH_INLINE __m128 SquareRootPS(__m128 x)
				{
					return _mm_mul_ps(x, ReciprocalSquareRootPS(x));
				}

				static __MYE_MATH_INLINE __m128 SquareRootSS(__m128 x)
				{
					return _mm_mul_ss(x, ReciprocalSquareRootSS(x));
				}

				static __MYE_MATH_INLINE __m128 ReciprocalPS(__m128 x)
				{
					return ReciprocalPSStep(x, _mm_rcp_ps(x));
				}

				static __MYE_MATH_INLINE __m128 ReciprocalSS(__m128 x)
				{
					return ReciprocalSSStep(x, _mm_rcp_ss(x));
				}

				static __MYE_MATH_INLINE __m128 ReciprocalSquareRootPS(__m128 x)
				{
					return ReciprocalSquareRootPSStep(x, _mm_rsqrt_ps(x));
				}

				static __MYE_MATH_INLINE __m128 ReciprocalSquareRootSS(__m128 x)
				{
					return ReciprocalSquareRootSSStep(x, _mm_rsqrt_ss(x));
				}

			private:

				static __MYE_MATH_INLINE __m128 ReciprocalPSStep(__m128 x, __m128 yi)
				{
					__m128 t0 = _mm_mul_ps(_mm_mul_ps(x, yi), yi);
					__m128 t1 = _mm_add_ps(yi, yi);
					return _mm_sub_ps(t1, t0);
				}

				static __MYE_MATH_INLINE __m128 ReciprocalSSStep(__m128 x, __m128 yi)
				{
					__m128 t0 = _mm_mul_ss(_mm_mul_ss(x, yi), yi);
					__m128 t1 = _mm_add_ss(yi, yi);
					return _mm_sub_ss(t1, t0);
				}

				static __MYE_MATH_INLINE __m128 ReciprocalSquareRootPSStep(__m128 x, __m128 yi)
				{
					__m128 t0 = _mm_mul_ps(_mm_mul_ps(x, yi), yi);
					__m128 yn = _mm_mul_ps(_mm_sub_ps(_mm_set1_ps(3.0f), t0), _mm_mul_ps(_mm_set1_ps(0.5f), yi));
					return SSENewtonRaphson<N - 1>::ReciprocalSquareRootSSStep(x, yn);
				}

				static __MYE_MATH_INLINE __m128 ReciprocalSquareRootSSStep(__m128 x, __m128 yi)
				{
					__m128 t0 = _mm_mul_ss(_mm_mul_ss(x, yi), yi);
					__m128 yn = _mm_mul_ss(_mm_sub_ss(_mm_set_ss(3.0f), t0), _mm_mul_ss(_mm_set_ss(0.5f), yi));
					return SSENewtonRaphson<N - 1>::ReciprocalSquareRootSSStep(x, yn);
				}

				template <int K> friend struct SSENewtonRaphson;

			};

			template <>
			struct SSENewtonRaphson<0>
			{

			private:

				static __MYE_MATH_INLINE __m128 ReciprocalSquareRootPSStep(__m128 x, __m128 yi)
				{
					return yi;
				}

				static __MYE_MATH_INLINE __m128 ReciprocalSquareRootSSStep(__m128 x, __m128 yi)
				{
					return yi;
				}

				template <int K> friend struct SSENewtonRaphson;

			};

			__MYE_MATH_INLINE __m128 SumComponents(__m128 v)
			{

				// (v2, v3, v2, v3)
				__m128 t0 = _mm_movehl_ps(v, v);

				// (v0 + v2, v1 + v3, ...)
				__m128 t1 = _mm_add_ps(t0, v);

				// (v1 + v3, ...)
				__m128 t2 = _mm_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 1, 1, 1));

				return _mm_add_ps(t1, t2);

			}

			__MYE_MATH_INLINE __m128 SumComponents(__m128 a, __m128 b)
			{

				// (a2, a3, a2, a3)
				__m128 t0 = _mm_movehl_ps(a, a);

				// (a0 + a2, a1 + a3, ...)
				__m128 t1 = _mm_add_ps(t0, a);

				// (b0, b1, b0, b1)
				__m128 t2 = _mm_movelh_ps(b, b);

				// (..., b0 + b2, b1 + b3)
				__m128 t3 = _mm_add_ps(t2, b);

				// (a0 + a2, a1 + a3, b0 + b2, b1 + b3)
				__m128 t4 = _mm_shuffle_ps(t1, t3, _MM_SHUFFLE(3, 2, 1, 0));

				// (a1 + a3, a0 + a2, b1 + b3, b0 + b2)
				__m128 t5 = _mm_shuffle_ps(t4, t4, _MM_SHUFFLE(2, 3, 0, 1));

				// (a0 + a1 + a2 + a3, a0 + a1 + a2 + a3, b0 + b1 + b2 + b3, b0 + b1 + b2 + b3)
				__m128 t6 = _mm_add_ps(t4, t5);

				return t6;

			}

			__MYE_MATH_INLINE __m128 SplatSS(__m128 v)
			{
				return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
			}

			__MYE_MATH_INLINE __m128 SSEFloor(__m128 v)
			{

				__m128i v0 = _mm_setzero_si128();
				__m128i v1 = _mm_cmpeq_epi32(v0, v0);

				__m128i ji = _mm_srli_epi32(v1, 25);
				__m128 j = * (__m128 *) & _mm_slli_epi32(ji, 23);

				__m128i i = _mm_cvttps_epi32(v);
				__m128 fi = _mm_cvtepi32_ps(i);
				__m128 igx = _mm_cmpgt_ps(fi, v);

				j = _mm_and_ps(igx, j);

				return _mm_sub_ps(fi, j);

			}

			__MYE_MATH_INLINE __m128 SSECeil(__m128 v){

				__m128i v0 = _mm_setzero_si128();
				__m128i v1 = _mm_cmpeq_epi32(v0, v0);

				__m128i ji = _mm_srli_epi32(v1, 25);
				__m128 j = *(__m128*)&_mm_slli_epi32(ji, 23);

				__m128i i = _mm_cvttps_epi32(v);
				__m128 fi = _mm_cvtepi32_ps(i);
				__m128 igx = _mm_cmplt_ps(fi, v);

				j = _mm_and_ps(igx, j);

				return _mm_add_ps(fi, j);

			}

			__MYE_MATH_INLINE __m128 SSERound(__m128 v){

				__m128 v0 = _mm_setzero_ps();
				__m128 v1 = _mm_cmpeq_ps(v0, v0);

				__m128 vNearest2 = * (__m128 *) & _mm_srli_epi32(* (__m128i *) &v1, 2);

				__m128i i = _mm_cvttps_epi32(v);

				__m128 aTrunc = _mm_cvtepi32_ps(i);

				__m128 rmd = _mm_sub_ps(v, aTrunc);
				__m128 rmd2 = _mm_mul_ps(rmd, vNearest2);
				__m128i rmd2i = _mm_cvttps_epi32(rmd2);
				__m128 rmd2Trunc = _mm_cvtepi32_ps(rmd2i);

				__m128 r =_mm_add_ps(aTrunc, rmd2Trunc);

				return r;

			}

			__MYE_MATH_INLINE __m128 SSEDot2(__m128 a, __m128 b)
			{
				__m128 t0 = _mm_mul_ps(a, b);
				__m128 t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(3, 2, 0, 1));
				__m128 t2 = _mm_add_ps(t0, t1);
				return t2;
			}

			__MYE_MATH_INLINE __m128 SSEDot3(__m128 a, __m128 b)
			{
				return _mm_dp_ps(a, b, 0x71);
			}

			__MYE_MATH_INLINE __m128 SSEDot4(__m128 a, __m128 b)
			{
				return _mm_dp_ps(a, b, 0xF1);
			}

			__MYE_MATH_INLINE __m128 SSEDeterminant4(const __m128 * m)
			{

				/* GLM sse_det_ps */

				//T SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
				//T SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
				//T SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
				//T SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
				//T SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
				//T SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];


				__m128 SubE;
				__m128 SubF;
				{

					// First 2 columns
					__m128 Swp2A = _mm_shuffle_ps(m[2], m[2], _MM_SHUFFLE(0, 1, 1, 2));
					__m128 Swp3A = _mm_shuffle_ps(m[3], m[3], _MM_SHUFFLE(3, 2, 3, 3));

					// Second 2 columns
					__m128 Swp2B = _mm_shuffle_ps(m[2], m[2], _MM_SHUFFLE(3, 2, 3, 3));
					__m128 Swp3B = _mm_shuffle_ps(m[3], m[3], _MM_SHUFFLE(0, 1, 1, 2));

					// Last 2 rows
					__m128 Swp2C = _mm_shuffle_ps(m[2], m[2], _MM_SHUFFLE(0, 0, 1, 2));
					__m128 Swp3C = _mm_shuffle_ps(m[3], m[3], _MM_SHUFFLE(1, 2, 0, 0));

					__m128 MulA = _mm_mul_ps(Swp2A, Swp3A);
					__m128 MulB = _mm_mul_ps(Swp2B, Swp3B);
					__m128 MulC = _mm_mul_ps(Swp2C, Swp3C);

					// Columns subtraction
					SubF = _mm_sub_ps(_mm_movehl_ps(MulC, MulC), MulC);
					SubE = _mm_sub_ps(MulA, MulB);

				}

				//detail::tvec4<T> DetCof(
				//	+ (m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02),
				//	- (m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04),
				//	+ (m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05),
				//	- (m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05));

				__m128 DetCof;
				{

					__m128 SubFacA = _mm_shuffle_ps(SubE, SubE, _MM_SHUFFLE(2, 1, 0, 0));
					__m128 SwpFacA = _mm_shuffle_ps(m[1], m[1], _MM_SHUFFLE(0, 0, 0, 1));

					__m128 SubTmpB = _mm_shuffle_ps(SubE, SubF, _MM_SHUFFLE(0, 0, 3, 1));
					__m128 SubFacB = _mm_shuffle_ps(SubTmpB, SubTmpB, _MM_SHUFFLE(3, 1, 1, 0));//SubF[0], SubE[3], SubE[3], SubE[1];
					__m128 SwpFacB = _mm_shuffle_ps(m[1], m[1], _MM_SHUFFLE(1, 1, 2, 2));

					__m128 SubTmpC = _mm_shuffle_ps(SubE, SubF, _MM_SHUFFLE(1, 0, 2, 2));
					__m128 SubFacC = _mm_shuffle_ps(SubTmpC, SubTmpC, _MM_SHUFFLE(3, 3, 2, 0));
					__m128 SwpFacC = _mm_shuffle_ps(m[1], m[1], _MM_SHUFFLE(2, 3, 3, 3));

					__m128 MulFacA = _mm_mul_ps(SwpFacA, SubFacA);
					__m128 MulFacB = _mm_mul_ps(SwpFacB, SubFacB);
					__m128 MulFacC = _mm_mul_ps(SwpFacC, SubFacC);

					__m128 SubRes = _mm_sub_ps(MulFacA, MulFacB);
					__m128 AddRes = _mm_add_ps(SubRes, MulFacC);

					DetCof = _mm_mul_ps(AddRes, _mm_setr_ps(1.0f, -1.0f, 1.0f, -1.0f));


				}

				return _mm_dp_ps(m[0], DetCof, 0xF1);

			}

			__MYE_MATH_INLINE __m128 SSEDeterminant3(const __m128 * m)
			{

				__m128 t0 = _mm_shuffle_ps(m[1], m[1], _MM_SHUFFLE(3, 0, 2, 1));
				__m128 t1 = _mm_shuffle_ps(m[2], m[2], _MM_SHUFFLE(3, 1, 0, 2));

				__m128 t2 = _mm_shuffle_ps(m[1], m[1], _MM_SHUFFLE(3, 1, 0, 2));
				__m128 t3 = _mm_shuffle_ps(m[2], m[2], _MM_SHUFFLE(3, 0, 2, 1));

				__m128 t4 = _mm_mul_ps(m[0], t0);
				__m128 t5 = _mm_dp_ps(t4, t1, 0x71);

				__m128 t7 = _mm_mul_ps(m[0], t2);
				__m128 t8 = _mm_dp_ps(t7, t3, 0x71);

				return _mm_sub_ss(t5, t8);

			}

			__MYE_MATH_INLINE __m128 SSEDeterminant2(__m128 m)
			{

				__m128 t0 = _mm_shuffle_ps(m, m, _MM_SHUFFLE(0, 1, 2, 3));
				__m128 t1 = _mm_mul_ps(m, t0);

				return _mm_hsub_ps(t1, t1);

			}

			__MYE_MATH_INLINE __m128 SSEDeterminant2(__m128 m1, __m128 m2)
			{

				__m128 t0 = _mm_shuffle_ps(m1, m2, _MM_SHUFFLE(1, 0, 1, 0));
				__m128 t1 = _mm_shuffle_ps(m1, m2, _MM_SHUFFLE(2, 3, 2, 3));
				__m128 t2 = _mm_mul_ps(t0, t1);
				__m128 t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2, 3, 0, 1));

				return _mm_sub_ps(t2, t3);
				
			}

			/* Intel Inverse of 4x4 Matrix */

			__MYE_MATH_INLINE void SSEInverse4(const __m128 * src, __m128 * rows)
			{

				__m128   minor0, minor1, minor2, minor3;
				__m128   row0, row1, row2, row3;
				__m128   det, tmp1;

				row0 = src[0];
				row3 = src[1];
				row2 = src[2];
				row1 = src[3];

				//tmp1     = _mm_loadh_pi(_mm_loadl_pi(tmp1, (const __m64 *)(src)),     (const __m64 *)(src + 4));
				//row1     = _mm_loadh_pi(_mm_loadl_pi(row1, (const __m64 *)(src + 8)), (const __m64 *)(src + 12));
				//row0     = _mm_shuffle_ps(tmp1, row1, 0x88);
				//row1     = _mm_shuffle_ps(row1, tmp1, 0xDD);
				//tmp1     = _mm_loadh_pi(_mm_loadl_pi(tmp1, (const __m64 *)(src + 2)),  (const __m64 *)(src + 6));
				//row3     = _mm_loadh_pi(_mm_loadl_pi(row3, (const __m64 *)(src + 10)), (const __m64 *)(src + 14));
				//row2     = _mm_shuffle_ps(tmp1, row3, 0x88);
				//row3     = _mm_shuffle_ps(row3, tmp1, 0xDD);
				//                  -----------------------------------------------
				tmp1     = _mm_mul_ps(row2, row3);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				minor0   = _mm_mul_ps(row1, tmp1);
				minor1   = _mm_mul_ps(row0, tmp1);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				minor0   = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
				minor1   = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
				minor1   = _mm_shuffle_ps(minor1, minor1, 0x4E);
				//                  -----------------------------------------------
				tmp1     = _mm_mul_ps(row1, row2);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				minor0   = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
				minor3   = _mm_mul_ps(row0, tmp1);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				minor0   = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
				minor3   = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
				minor3   = _mm_shuffle_ps(minor3, minor3, 0x4E);
				//                  -----------------------------------------------
				tmp1     = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				row2     = _mm_shuffle_ps(row2, row2, 0x4E);
				minor0   = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
				minor2   = _mm_mul_ps(row0, tmp1);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				minor0   = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
				minor2   = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
				minor2   = _mm_shuffle_ps(minor2, minor2, 0x4E);
				//                  -----------------------------------------------
				tmp1     = _mm_mul_ps(row0, row1);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				minor2   = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
				minor3   = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				minor2   = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
				minor3   = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
				//                  -----------------------------------------------
				tmp1     = _mm_mul_ps(row0, row3);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				minor1   = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
				minor2   = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				minor1   = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
				minor2   = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
				//                  -----------------------------------------------
				tmp1     = _mm_mul_ps(row0, row2);
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				minor1   = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
				minor3   = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
				tmp1     = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				minor1   = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
				minor3   = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
				//                  -----------------------------------------------
				det      = _mm_mul_ps(row0, minor0);
				det      = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
				det      = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
				tmp1     = _mm_rcp_ss(det);
				det      = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
				det      = _mm_shuffle_ps(det, det, 0x00);
				minor0   = _mm_mul_ps(det, minor0);
				minor1   = _mm_mul_ps(det, minor1);
				minor2   = _mm_mul_ps(det, minor2);
				minor3   = _mm_mul_ps(det, minor3);

				rows[0] = minor0;
				rows[1] = minor1;
				rows[2] = minor2;
				rows[3] = minor3;

			}

		}


	}

}
