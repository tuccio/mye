#pragma once

#include <type_traits>
#include <limits>
#include <cstring>

#include <cstdint>

namespace mye
{

	namespace core
	{

		typedef uint8_t Byte;

		template <typename T, typename Enable = void>
		struct FloatInfo;

		template <>
		struct FloatInfo<float, typename std::enable_if<std::numeric_limits<float>::is_iec559>::type>
		{
			
			typedef std::integral_constant<int, 23> mantissa;
			typedef std::integral_constant<int, 8>  exponent;
			typedef std::integral_constant<int, 32> bits;

			typedef uint32_t integral;

		};

		template <>
		struct FloatInfo<double, typename std::enable_if<std::numeric_limits<double>::is_iec559>::type>
		{

			typedef std::integral_constant<int, 52> mantissa;
			typedef std::integral_constant<int, 11> exponent;
			typedef std::integral_constant<int, 64> bits;

			typedef uint64_t integral;

		};

		template <typename T, typename Enable = void>
		struct PrimitiveSerializer;

		// TODO: Endianness

		template <>
		struct PrimitiveSerializer<uint32_t>
		{

			static void Write(uint32_t i, Byte * buffer)
			{
				memcpy((void*) buffer, (void*) &i, 4);
			}

			static uint32_t Read(const Byte * data)
			{
				return * (const uint32_t *) data;
			}

		};

		template <>
		struct PrimitiveSerializer<uint64_t>
		{

			static void Write(uint64_t i, Byte * buffer)
			{
				memcpy((void*) buffer, (void*) &i, 8);
			}

			static uint64_t Read(const Byte * data)
			{
				return * (const uint64_t *) data;
			}

		};

		template <typename T>
		struct PrimitiveSerializer<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
		{

			static void Write(T f, Byte * buffer)
			{

				if (f == 0.0f)
				{
					PrimitiveSerializer<FloatInfo<T>::integral>::Write(0, buffer);
				}
				else
				{

					FloatInfo<T>::integral sign, exponent, mantissa, result;

					T fnorm;

					if (f < 0.0f)
					{
						sign = 1;
						fnorm = -f;
					}
					else
					{
						sign = 0;
						fnorm = f;
					}

					int shift = 0;

					while (fnorm >= 2.0f)
					{
						fnorm *= 0.5f;
						shift++;
					}

					while (fnorm < 1.0f)
					{
						fnorm *= 2.0f;
						shift--;
					}

					fnorm = fnorm - 1.0f;

					mantissa = fnorm *  ((1LL << FloatInfo<T>::mantissa::value) + 0.5f);
					exponent = shift + ((1LL << (FloatInfo<T>::exponent::value - 1)) - 1);

					result = (sign << (FloatInfo<T>::bits::value - 1)) |
						(exponent << (FloatInfo<T>::mantissa::value)) |
						mantissa;

					PrimitiveSerializer<FloatInfo<T>::integral>::Write(result, buffer);

				}				

			}

			static T Read(const Byte * data)
			{

				FloatInfo<T>::integral i = PrimitiveSerializer<FloatInfo<T>::integral>::Read(data);

				if (i == 0)
				{
					return T(0.0f);
				}

				T result;
				int shift;
				FloatInfo<T>::integral bias;

				result = (i & ((1LL << FloatInfo<T>::mantissa::value) - 1));
				result /= (1LL << FloatInfo<T>::mantissa::value);
				result += 1.0f;

				bias = (1 << (FloatInfo<T>::exponent::value - 1)) - 1;
				shift = ((i >> FloatInfo<T>::mantissa::value) & ((1LL<<FloatInfo<T>::exponent::value) - 1)) - bias;

				while (shift > 0)
				{
					result *= 2.0;
					shift--;
				}
				while (shift < 0)
				{
					result /= 2.0;
					shift++;
				}

				result *= (i >> (FloatInfo<T>::bits::value - 1)) & 1 ? -1.0 : 1.0;

				return result;

			}

		};

	}

}