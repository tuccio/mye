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
		__MYE_MATH_INLINE T Radians(T && degAngle)
		{
			return degAngle * T(__MYE_MATH_PI_DIV_180);
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE T Degrees(T && radAngle)
		{
			return radAngle * T(__MYE_MATH_180_DIV_PI);
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE detail::SFINAEStrippedFP<T> Tangent(T && angle)
		{
			return detail::TrigonometryFunctions<T>::Tangent(std::forward<T>(angle));
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE detail::SFINAEStrippedFP<T> Arctangent(T && x)
		{
			return detail::TrigonometryFunctions<T>::Arctangent(std::forward<T>(x));
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE detail::SFINAEStrippedFP<T> Cosine(T && angle)
		{
			return detail::TrigonometryFunctions<T>::Cosine(std::forward<T>(angle));
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE detail::SFINAEStrippedFP<T> Arccosine(T && x)
		{
			return detail::TrigonometryFunctions<T>::Arccosine(std::forward<T>(x));
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE detail::SFINAEStrippedFP<T> Sine(T && angle)
		{
			return detail::TrigonometryFunctions<T>::Sine(std::forward<T>(angle));
		}

		template <typename T = mye::math::Real>
		__MYE_MATH_INLINE detail::SFINAEStrippedFP<T> Arcsine(T && x)
		{
			return detail::TrigonometryFunctions<T>::Arcsine(std::forward<T>(x));
		}

	}

}