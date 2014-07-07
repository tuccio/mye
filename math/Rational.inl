namespace mye
{

	namespace math
	{

		template <typename T>
		Rational<T, typename std::enable_if<std::is_integral<T>::value>::type>::Rational(void)
		{

		}

		template <typename T>
		Rational<T, typename std::enable_if<std::is_integral<T>::value>::type>::Rational(T numerator, T denominator) :
			m_numerator(numerator),
			m_denominator(denominator)
		{

		}

		template <typename T>
		T Rational<T, typename std::enable_if<std::is_integral<T>::value>::type>::GetNumerator(void) const
		{
			return m_numerator;;
		}

		template <typename T>
		void Rational<T, typename std::enable_if<std::is_integral<T>::value>::type>::SetNumerator(const T &numerator)
		{
			m_numerator = numerator;
		}

		template <typename T>
		T Rational<T, typename std::enable_if<std::is_integral<T>::value>::type>::GetDenominator(void) const
		{
			return m_denominator;
		}

		template <typename T>
		void Rational<T, typename std::enable_if<std::is_integral<T>::value>::type>::SetDenominator(const T &denominator)
		{
			m_denominator = denominator;
		}

	}

}