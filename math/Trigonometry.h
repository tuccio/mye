#include <cmath>
#include <type_traits>

namespace mye
{

	namespace math
	{

		template <typename T = mye::math::Real>
		typename std::enable_if<std::is_floating_point<T>::value, T>::type Pi(void)
		{
			return T(3.14159265358979323846);
		}

		template <typename T = mye::math::Real>
		typename std::enable_if<std::is_floating_point<T>::value, T>::type TwoPi(void)
		{
			return T(6.28318530717958647693);
		}

		template <typename T>
		inline T Radians(T degAngle)
		{
			return degAngle * Pi<T>() / T(180);
		}

		template <typename T>
		inline T Degrees(T radAngle)
		{
			return radAngle * T(180) / Pi<T>();
		}

		template <typename T>
		inline T Tangent(T angle)
		{
			return ::tan((angle));
		}

		template <typename T>
		inline T Arctangent(T x)
		{
			return (::atan(x));
		}

		template <typename T>
		inline T Cosine(T angle)
		{
			return ::cos((angle));
		}

		template <typename T>
		inline T Arccosine(T x)
		{
			return (::acos(x));
		}

		template <typename T>
		inline T Sine(T angle)
		{
			return ::sin((angle));
		}

		template <typename T>
		inline T Arcsine(T x)
		{
			return (::asin(x));
		}



	}

}