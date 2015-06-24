#pragma once

#include <cstdint>

#include "Common.h"

namespace mye
{

	namespace algorithms
	{

		typedef uint64_t MortonCode;

		namespace detail
		{

			__MYE_ALGORITHMS_INLINE MortonCode MortonCompact3(MortonCode code)
			{

				uint64_t t = code & 0x1249249249249249;

				t = (t ^ (t >>  2)) & 0x30C30C30C30C30C3;
				t = (t ^ (t >>  4)) & 0x100F00F00F00F00F;
				t = (t ^ (t >>  8)) & 0x1F0000FF0000FF;
				t = (t ^ (t >> 16)) & 0x1F00000000FFFF;

				return t;

			}

			__MYE_ALGORITHMS_INLINE MortonCode MortonSplit3(unsigned int x)
			{

				uint64_t t = x & 0x1FFFFF;

				t = (t | t << 32) & 0x1F00000000FFFF;
				t = (t | t << 16) & 0x1F0000FF0000FF;
				t = (t | t << 8) & 0x100F00F00F00F00F;
				t = (t | t << 4) & 0x10C30C30C30C30C3;
				t = (t | t << 2) & 0x1249249249249249;

				return t;

			}

#ifdef MYE_USE_SSE

			union SSERegister {
				__m128i __128i;
				uint64_t __ui64[2];
			};
			
			__MYE_ALGORITHMS_INLINE __m128i MortonCompact3Low(__m128i code)
			{

				/*uint64_t t = code & 0x1249249249249249;

				t = (t ^ (t >>  2)) & 0x30C30C30C30C30C3;
				t = (t ^ (t >>  4)) & 0x100F00F00F00F00F;
				t = (t ^ (t >>  8)) & 0x1F0000FF0000FF;
				t = (t ^ (t >> 16)) & 0x1F00000000FFFF;*/

				static const __m128i mask0 = _mm_set_epi32(0x0, 0x0, 0x12492492, 0x49249249);
				static const __m128i mask1 = _mm_set_epi32(0x0, 0x0, 0x30C30C30, 0xC30C30C3);
				static const __m128i mask2 = _mm_set_epi32(0x0, 0x0, 0x100F00F0, 0x0F00F00F);
				static const __m128i mask3 = _mm_set_epi32(0x0, 0x0, 0x001F0000, 0xFF0000FF);
				static const __m128i mask4 = _mm_set_epi32(0x0, 0x0, 0x001F0000, 0x0000FFFF);

				__m128i t = _mm_and_si128(code, mask0);

				t = _mm_and_si128(_mm_xor_si128(t, _mm_srli_epi64(t, 2)),  mask1);
				t = _mm_and_si128(_mm_xor_si128(t, _mm_srli_epi64(t, 4)),  mask2);
				t = _mm_and_si128(_mm_xor_si128(t, _mm_srli_epi64(t, 8)),  mask3);
				t = _mm_and_si128(_mm_xor_si128(t, _mm_srli_epi64(t, 16)), mask4);

				return t;

			}

			__MYE_ALGORITHMS_INLINE __m128i MortonSplit3Low(__m128i x)
			{

				__m128i mask21 = _mm_set_epi32(0x0, 0x0, 0x0, 0x1FFFFF);

				__m128i t = _mm_and_si128(x, mask21);

				static const __m128i mask0 = _mm_set_epi32(0x0, 0x0, 0x001F0000, 0x0000FFFF);
				static const __m128i mask1 = _mm_set_epi32(0x0, 0x0, 0x001F0000, 0xFF0000FF);
				static const __m128i mask2 = _mm_set_epi32(0x0, 0x0, 0x100F00F0, 0x0F00f00F);
				static const __m128i mask3 = _mm_set_epi32(0x0, 0x0, 0x10C30C30, 0xC30C30C3);
				static const __m128i mask4 = _mm_set_epi32(0x0, 0x0, 0x12492492, 0x49249249);

				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 32)), mask0);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 16)), mask1);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 8)),  mask2);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 4)),  mask3);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 2)),  mask4);

				return t;

			}

			__MYE_ALGORITHMS_INLINE __m128i MortonSplit3LowHigh(__m128i x)
			{

				__m128i mask21 = _mm_set_epi32(0x0, 0x1FFFFF, 0x0, 0x1FFFFF);

				__m128i t = _mm_and_si128(x, mask21);

				static const __m128i mask0 = _mm_set_epi32(0x001F0000, 0x0000FFFF, 0x001F0000, 0x0000FFFF);
				static const __m128i mask1 = _mm_set_epi32(0x001F0000, 0xFF0000FF, 0x001F0000, 0xFF0000FF);
				static const __m128i mask2 = _mm_set_epi32(0x100F00F0, 0x0F00f00F, 0x100F00F0, 0x0F00f00F);
				static const __m128i mask3 = _mm_set_epi32(0x10C30C30, 0xC30C30C3, 0x10C30C30, 0xC30C30C3);
				static const __m128i mask4 = _mm_set_epi32(0x12492492, 0x49249249, 0x12492492, 0x49249249);				

				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 32)), mask0);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 16)), mask1);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 8)),  mask2);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 4)),  mask3);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 2)),  mask4);

				return t;

			}

#endif

		}

		__MYE_ALGORITHMS_INLINE MortonCode MortonEncode3Slow(unsigned int x, unsigned int y, unsigned int z)
		{

			return detail::MortonSplit3(x) |
				detail::MortonSplit3(y) << 1 |
				detail::MortonSplit3(z) << 2;

		}

		__MYE_ALGORITHMS_INLINE void MortonDecode3Slow(MortonCode code, unsigned int & x, unsigned int & y, unsigned int & z)
		{

			x = detail::MortonCompact3(code);
			y = detail::MortonCompact3(code >> 1);
			z = detail::MortonCompact3(code >> 2);

		}


#ifdef MYE_USE_SSE

		// Morton code obtained interleaving as in zyx

		__MYE_ALGORITHMS_INLINE MortonCode MortonEncode3Fast(unsigned int x, unsigned int y, unsigned int z)
		{

			/*__m128i xi = detail::MortonSplit3Low(_mm_set_epi32(0x0, 0x0, 0x0, x));
			__m128i yi = detail::MortonSplit3Low(_mm_set_epi32(0x0, 0x0, 0x0, y));
			__m128i zi = detail::MortonSplit3Low(_mm_set_epi32(0x0, 0x0, 0x0, z));

			yi = _mm_slli_epi64(yi, 1);
			zi = _mm_slli_epi64(zi, 2);

			union {
				__m128i __128i; uint64_t __ui64[2];
			} r = { _mm_or_si128(_mm_or_si128(xi, yi), zi) };

			return r.__ui64[0];*/

			__m128i xiyi = detail::MortonSplit3LowHigh(_mm_set_epi32(0x0, y, 0x0, x));
			__m128i zi   = detail::MortonSplit3Low(_mm_set_epi32(0x0, 0x0, 0x0, z));

			__m128i yi   = _mm_shuffle_epi32(xiyi, _MM_SHUFFLE(3, 2, 3, 2));

			yi = _mm_slli_epi64(yi, 1);
			zi = _mm_slli_epi64(zi, 2);

			detail::SSERegister r = { _mm_or_si128(_mm_or_si128(xiyi, yi), zi) };

			return r.__ui64[0];

		}

		__MYE_ALGORITHMS_INLINE void MortonDecode3Fast(MortonCode code, unsigned int & x, unsigned int & y, unsigned int & z)
		{

			detail::SSERegister rx, ry, rz;

			rx.__ui64[0] = code;

			ry = { _mm_srli_epi64(rx.__128i, 1) };
			rz = { _mm_srli_epi64(rx.__128i, 2) };

			rx = { detail::MortonCompact3Low(rx.__128i) };
			ry = { detail::MortonCompact3Low(ry.__128i) };
			rz = { detail::MortonCompact3Low(rz.__128i) };

			x = rx.__ui64[0];
			y = ry.__ui64[0];
			z = rz.__ui64[0];

		}

		__MYE_ALGORITHMS_INLINE MortonCode MortonEncode3(unsigned int x, unsigned int y, unsigned int z)
		{
			return MortonEncode3Fast(x, y, z);
		}

		__MYE_ALGORITHMS_INLINE void MortonDecode3(MortonCode code, unsigned int & x, unsigned int & y, unsigned int & z)
		{
			MortonDecode3Fast(code, x, y, z);
		}

#else

		__MYE_ALGORITHMS_INLINE MortonCode MortonEncode3(unsigned int x, unsigned int y, unsigned int z)
		{
			return MortonEncode3Slow(x, y, z);
		}

		__MYE_ALGORITHMS_INLINE void MortonDecode3(MortonCode code, unsigned int & x, unsigned int & y, unsigned int & z)
		{
			MortonDecode3Slow(code, x, y, z);
		}

#endif

		



//#endif

	}

}