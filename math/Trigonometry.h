#include <cmath>
#include <type_traits>

#include "Constants.h"
#include "TrigonometryImpl.h"
#include "Utils.h"

namespace mye
{

	namespace math
	{

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE T Pi(void)
		{
			return T(__MYE_MATH_PI);
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE T TwoPi(void)
		{
			return T(__MYE_MATH_TWOPI);
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE T Radians(const T & degAngle)
		{
			return degAngle * T(__MYE_MATH_PI_DIV_180);
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE T Degrees(const T & radAngle)
		{
			return radAngle * T(__MYE_MATH_180_DIV_PI);
		}
		
		/* Scalar functions */

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEScalar<T> Tangent(T angle)
		{
			return detail::TrigonometryFunctions<T>::Tangent(angle);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEScalar<T> Arctangent(T x)
		{
			return detail::TrigonometryFunctions<T>::Arctangent(x);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEScalar<T> Cosine(T angle)
		{
			return detail::TrigonometryFunctions<T>::Cosine(angle);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEScalar<T> Arccosine(T x)
		{
			return detail::TrigonometryFunctions<T>::Arccosine(x);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEScalar<T> Sine(T angle)
		{
			return detail::TrigonometryFunctions<T>::Sine(angle);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEScalar<T> Arcsine(T x)
		{
			return detail::TrigonometryFunctions<T>::Arcsine(x);
		}

		/* Vector functions */

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEVector<T> Tangent(const T & angle)
		{
			return detail::TrigonometryFunctions<T>::Tangent(angle);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEVector<T> Arctangent(const T & x)
		{
			return detail::TrigonometryFunctions<T>::Arctangent(x);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEVector<T> Cosine(const T & angle)
		{
			return detail::TrigonometryFunctions<T>::Cosine(angle);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEVector<T> Arccosine(const T & x)
		{
			return detail::TrigonometryFunctions<T>::Arccosine(x);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEVector<T> Sine(const T & angle)
		{
			return detail::TrigonometryFunctions<T>::Sine(angle);
		}

		template <typename T>
		__MYE_MATH_INLINE detail::SFINAEVector<T> Arcsine(const T & x)
		{
			return detail::TrigonometryFunctions<T>::Arcsine(x);
		}

	}

}