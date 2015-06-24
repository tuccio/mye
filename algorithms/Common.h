#pragma once

#define __MYE_ALGORITHMS_INLINE inline

#include <cstdint>

#include <intrin.h>

#ifdef MYE_USE_SSE

#include <xmmintrin.h>
#include <mmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>

#endif

#define __MYE_ALGORITHMS_BSR_INVALID (0xFFFFFFFF)

namespace mye
{

	namespace algorithms
	{

		namespace detail
		{


			__MYE_ALGORITHMS_INLINE uint32_t LeadingZeros(uint64_t x)
			{

#ifdef _MSC_VER

				union
				{
					uint64_t ui64;
					uint32_t ui32[2];
				} t = { x };

				unsigned long bsrHigh;
				unsigned long bsrLow;

				unsigned char okHigh = _BitScanReverse(&bsrHigh, t.ui32[1]);
				unsigned char okLow  = _BitScanReverse(&bsrLow,  t.ui32[0]);

				unsigned int lz;

				if (okHigh)
				{
					lz = 31 - bsrHigh;
				}
				else if (okLow)
				{
					lz = 63 - bsrLow;
				}
				else
				{
					lz = 64;
				}

				return lz;

#endif

			}

			__MYE_ALGORITHMS_INLINE uint32_t MostSignificantOne(uint64_t x)
			{

#ifdef _MSC_VER

				union
				{
					uint64_t ui64;
					uint32_t ui32[2];
				} t = { x };

				unsigned long bsrHigh;
				unsigned long bsrLow;

				unsigned char okHigh = _BitScanReverse(&bsrHigh, t.ui32[1]);
				unsigned char okLow  = _BitScanReverse(&bsrLow, t.ui32[0]);

				unsigned int bsr;

				if (okHigh)
				{
					bsr = bsrHigh + 32;
				}
				else if (okLow)
				{
					bsr = bsrLow;
				}
				else
				{
					bsr = __MYE_ALGORITHMS_BSR_INVALID;
				}

				return bsr;

#endif

			}

		}

	}

}