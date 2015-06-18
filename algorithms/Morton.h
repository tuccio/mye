#pragma once

#include <cstdint>

#include "Common.h"

namespace mye
{

	namespace algorithms
	{

		typedef uint64_t MortonCode;

#ifndef MYE_USE_SSE

		namespace detail
		{

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

		}

		__MYE_ALGORITHMS_INLINE MortonCode Morton(unsigned int x, unsigned int y, unsigned int z)
		{

			return detail::MortonSplit3(x) |
				detail::MortonSplit3(y) << 1 |
				detail::MortonSplit3(z) << 2;

		}

#else

		namespace detail
		{

			__MYE_ALGORITHMS_INLINE __m128i MortonSplit3(__m128i x)
			{

				__m128i mask21 = _mm_set_epi32(0x0, 0x0, 0x0, 0x1FFFFF);

				__m128i t = _mm_and_si128(x, mask21);

				__m128i mask0 = _mm_set_epi32(0x0, 0x0, 0x1F0000,   0xFFFF);
				__m128i mask1 = _mm_set_epi32(0x0, 0x0, 0x1F0000,   0xFF0000FF);
				__m128i mask2 = _mm_set_epi32(0x0, 0x0, 0x100F00F0, 0x0F00f00F);
				__m128i mask3 = _mm_set_epi32(0x0, 0x0, 0x10C30C30, 0xC30C30C3);
				__m128i mask4 = _mm_set_epi32(0x0, 0x0, 0x12492492, 0x49249249);

				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 32)), mask0);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 16)), mask1);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 8)),  mask2);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 4)),  mask3);
				t = _mm_and_si128(_mm_or_si128(t, _mm_slli_epi64(t, 2)),  mask4);

				return t;

			}

		}

		__MYE_ALGORITHMS_INLINE MortonCode Morton(unsigned int x, unsigned int y, unsigned int z)
		{

			__m128i xi = detail::MortonSplit3(_mm_set_epi32(0x0, 0x0, 0x0, x));
			__m128i yi = detail::MortonSplit3(_mm_set_epi32(0x0, 0x0, 0x0, y));
			__m128i zi = detail::MortonSplit3(_mm_set_epi32(0x0, 0x0, 0x0, z));

			yi = _mm_slli_epi64(yi, 1);
			zi = _mm_slli_epi64(zi, 2);

			union { __m128i __128i; unsigned int __ui64[2]; } r = { _mm_or_si128(_mm_or_si128(xi, yi), zi) };

			return r.__ui64[0];
			

		}

#endif

	}

}