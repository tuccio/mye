#include <cmath>
#include <type_traits>

#define __MYE_MATH_PI    3.14159265358979323846
#define __MYE_MATH_TWOPI 6.28318530717958647693

namespace mye
{

	namespace math
	{

		template <typename T = mye::math::Real>
		typename std::enable_if<std::is_floating_point<T>::value, T>::type Pi(void)
		{
			return T(__MYE_MATH_PI);
		}

		template <typename T = mye::math::Real>
		typename std::enable_if<std::is_floating_point<T>::value, T>::type TwoPi(void)
		{
			return T(__MYE_MATH_TWOPI);
		}

		template <typename T>
		inline T Radians(T degAngle)
		{
			return degAngle * T(__MYE_MATH_PI) / T(180);
		}

		template <typename T>
		inline T Degrees(T radAngle)
		{
			return radAngle * T(180) / T(__MYE_MATH_PI);
		}

		template <typename T>
		inline Matrix<T, 3, 1> Degrees(const Matrix<T, 3, 1> &angles)
		{
			return Matrix<T, 3, 1>(Degrees(angles.x()), Degrees(angles.y()), Degrees(angles.z()));
		}

		template <typename T>
		inline Matrix<T, 3, 1> Radians(const Matrix<T, 3, 1> &angles)
		{
			return Matrix<T, 3, 1>(Radians(angles.x()), Radians(angles.y()), Radians(angles.z()));
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