#pragma once

namespace mye
{

	namespace core
	{

		enum
		{
			MYE_LITTLE_ENDIAN = 0x03020100u,
			MYE_BIG_ENDIAN = 0x00010203u
		};

		static const union { unsigned char bytes[4]; unsigned int value; } __mye_endianness = { { 0, 1, 2, 3 } };

#define MYE_ENDIANNESS (__mye_endianness.value)

		union __IEEE_754_FLOAT
		{
			int sign : 1;
			int exponent : 8;
			int mantissa : 23;
		};

		inline int HostToBigEndian(int x);
		inline int HostToLittleEndian(int x);

		inline unsigned int HostToBigEndian(unsigned int x);
		inline unsigned int HostToLittleEndian(unsigned int x);

		inline __IEEE_754_FLOAT HostToBigEndian(float x);
		inline __IEEE_754_FLOAT HostToLittleEndian(float x);

	}

}