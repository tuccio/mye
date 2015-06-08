#pragma once

#include <type_traits>

namespace mye
{

	namespace math
	{

		template <typename T = int, typename Enable = void>
		class Rational;

		template <typename T>
		class Rational<T, typename std::enable_if<std::is_integral<T>::value>::type>
		{

		public:

			Rational(void);
			Rational(T numerator, T denominator);

			inline T GetNumerator(void) const;
			inline void SetNumerator(const T &numerator);

			inline T GetDenominator(void) const;
			inline void SetDenominator(const T &numerator);

		private:

			T m_numerator;
			T m_denominator;

		};

	}

}

#include "Rational.inl"