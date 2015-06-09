#pragma once

#include "Constants.h"
#include "Declarations.h"
#include "SSE.h"

namespace mye
{

	namespace math
	{

		namespace detail
		{

			/*template <typename T, typename Enable = void>
			struct TrigonometryFunctions;*/

			template <typename T>
			struct TrigonometryFunctions<T, std::enable_if_t<std::is_floating_point<T>::value>>
			{

				__MYE_MATH_INLINE static T Tangent(T angle)
				{

					T w = SineWrap(angle);
					T x = (w < 0 ? -w : w);

					// Abramowitch & Stegun 4.3.101
					
					T x2  = x   * x;
					T x3  = x2  * x;
					T x5  = x3  * x2;
					T x7  = x5  * x2;
					T x9  = x7  * x2;
					T x11 = x9  * x2;
					T x13 = x11 * x2;

					T y = x + T(0.3333314036) * x3 + T(0.1333923995) * x5 +  T(0.0533740603) * x7 +
					          T(0.0245650893) * x9 + T(0.0029005250) * x11 + T(0.0095168091) * x13;

					// Change the sign according to the wrapped angle sign

					return (w < 0 ? -y : y);

				}

				__MYE_MATH_INLINE static T Arctangent(T x)
				{

					T x2 = x  * x;
					T x3 = x2 * x;
					T x5 = x3 * x2;
					T x7 = x5 * x2;
					T x9 = x7 * x2;
					
					T y = T(0.9998660) * x -
					      T(0.3302995) * x3 +
					      T(0.1801410) * x5 -
					      T(0.0851330) * x7 +
					      T(0.0208351) * x9;

					return y;

				}

				__MYE_MATH_INLINE static T Cosine(T angle)
				{

					T sign;
					T x = CosineWrap(angle, sign);

					// Abramowitz & Stegun 4.3.98

					T x2 = x * x;
					T x4 = x2 * x2;

					T y = 1 - T(0.49670) * x2 + T(0.03705) * x4;

					return sign * y;

				}

				__MYE_MATH_INLINE static T Arccosine(T x)
				{
					return return __MYE_MATH_HALFPI - Arcsine(x);
				}

				__MYE_MATH_INLINE static T Sine(T angle)
				{

					// Wrap the angle into [-pi/2, pi]

					T w = SineWrap(angle);
					T x = (w < 0 ? -w : w);

					// Abramowitz & Stegun 4.3.96

					T x2 = x  * x;
					T x3 = x2 * x;
					T x5 = x3 * x2;

					T y = x - T(0.16605) * x3 + T(0.00761) * x5;

					// Change the sign according to the wrapped angle sign

					return (w < 0 ? -y : y);

				}

				__MYE_MATH_INLINE static T Arcsine(T x)
				{
					
					// Abramowitz & Stegun 4.4.45

					T x2 = x * x;
					T x3 = x2 * x;

					T y = __MYE_MATH_HALFPI - Sqrt(1 - x) *
						(T(1.5707288) - T(0.2121144) * x + T(0.0742610) * x2 - T(0.0187293) * x3);

					return y;

				}

			private:

				__MYE_MATH_INLINE static T SineWrap(T x)
				{

					T quotient = T(__MYE_MATH_INV_TWOPI) * x;

					if (x >= T(0))
					{
						quotient = (T) ((int) (quotient + T(0.5)));
					}
					else
					{
						quotient = (T) ((int) (quotient - T(0.5)));
					}

					T y = x - T(__MYE_MATH_TWOPI) * quotient;

					if (y > T(__MYE_MATH_HALFPI))
					{
						y = T(__MYE_MATH_PI) - y;
					}
					else if (y < T(-__MYE_MATH_HALFPI))
					{
						y = T(-__MYE_MATH_PI) - y;
					}

					return y;

				}

				__MYE_MATH_INLINE static T CosineWrap(T x, T & sign)
				{

					T quotient = T(__MYE_MATH_INV_TWOPI) * x;

					if (x >= T(0))
					{
						quotient = (T) ((int) (quotient + T(0.5)));
					}
					else
					{
						quotient = (T) ((int) (quotient - T(0.5)));
					}

					T y = x - T(__MYE_MATH_TWOPI) * quotient;

					if (y > T(__MYE_MATH_HALFPI))
					{
						y = T(__MYE_MATH_PI) - y;
						sign = T(-1);
					}
					else if (y < T(-__MYE_MATH_HALFPI))
					{
						y = T(-__MYE_MATH_PI) - y;
						sign = T(-1);
					}
					else
					{
						sign = T(1);
					}

					return y;

				}

			};

			template <typename T>
			struct TrigonometryFunctions<Matrix<T, 2, 1>>
			{

				__MYE_MATH_INLINE static Matrix<T, 2, 1> Tangent(const Matrix<T, 2, 1> & angles)
				{

					return Matrix<T, 2, 1>(TrigonometryFunctions<T>::Tangent(angles.x()),
										   TrigonometryFunctions<T>::Tangent(angles.y()));

				}

				__MYE_MATH_INLINE static Matrix<T, 2, 1> Arctangent(const Matrix<T, 2, 1> & x)
				{

					return Matrix<T, 2, 1>(TrigonometryFunctions<T>::Arctangent(x.x()),
										   TrigonometryFunctions<T>::Arctangent(x.y()));

				}

				__MYE_MATH_INLINE static Matrix<T, 2, 1> Cosine(const Matrix<T, 2, 1> & angles)
				{

					return Matrix<T, 2, 1>(TrigonometryFunctions<T>::Cosine(angles.x()),
										   TrigonometryFunctions<T>::Cosine(angles.y()));

				}

				__MYE_MATH_INLINE static Matrix<T, 2, 1> Arccosine(T x)
				{

					return Matrix<T, 2, 1>(TrigonometryFunctions<T>::Arccosine(x.x()),
										   TrigonometryFunctions<T>::Arccosine(x.y()));

				}

				__MYE_MATH_INLINE static Matrix<T, 2, 1> Sine(const Matrix<T, 2, 1> & angles)
				{

					return Matrix<T, 2, 1>(TrigonometryFunctions<T>::Sine(angles.x()),
										   TrigonometryFunctions<T>::Sine(angles.y()));

				}

				__MYE_MATH_INLINE static T Arcsine(const Matrix<T, 2, 1> & x)
				{

					return Matrix<T, 2, 1>(TrigonometryFunctions<T>::Arcsine(x.x()),
										   TrigonometryFunctions<T>::Arcsine(x.y()));

				}

			};

			template <typename T>
			struct TrigonometryFunctions<Matrix<T, 3, 1>>
			{

				__MYE_MATH_INLINE static Matrix<T, 3, 1> Tangent(const Matrix<T, 3, 1> & angles)
				{

					return Matrix<T, 3, 1>(TrigonometryFunctions<T>::Tangent(angles.x()),
										   TrigonometryFunctions<T>::Tangent(angles.y()),
										   TrigonometryFunctions<T>::Tangent(angles.z()));

				}

				__MYE_MATH_INLINE static Matrix<T, 3, 1> Arctangent(const Matrix<T, 3, 1> & x)
				{

					return Matrix<T, 3, 1>(TrigonometryFunctions<T>::Arctangent(x.x()),
										   TrigonometryFunctions<T>::Arctangent(x.y()),
										   TrigonometryFunctions<T>::Arctangent(x.z()));

				}

				__MYE_MATH_INLINE static Matrix<T, 3, 1> Cosine(const Matrix<T, 3, 1> & angles)
				{

					return Matrix<T, 3, 1>(TrigonometryFunctions<T>::Cosine(angles.x()),
										   TrigonometryFunctions<T>::Cosine(angles.y()),
										   TrigonometryFunctions<T>::Cosine(angles.z()));

				}

				__MYE_MATH_INLINE static Matrix<T, 3, 1> Arccosine(T x)
				{

					return Matrix<T, 3, 1>(TrigonometryFunctions<T>::Arccosine(x.x()),
										   TrigonometryFunctions<T>::Arccosine(x.y()),
										   TrigonometryFunctions<T>::Arccosine(x.z()));

				}

				__MYE_MATH_INLINE static Matrix<T, 3, 1> Sine(const Matrix<T, 3, 1> & angles)
				{

					return Matrix<T, 3, 1>(TrigonometryFunctions<T>::Sine(angles.x()),
										   TrigonometryFunctions<T>::Sine(angles.y()),
										   TrigonometryFunctions<T>::Sine(angles.z()));

				}

				__MYE_MATH_INLINE static T Arcsine(const Matrix<T, 3, 1> & x)
				{

					return Matrix<T, 3, 1>(TrigonometryFunctions<T>::Arcsine(x.x()),
										   TrigonometryFunctions<T>::Arcsine(x.y()),
										   TrigonometryFunctions<T>::Arcsine(x.z()));

				}

			};

			template <typename T>
			struct TrigonometryFunctions<Matrix<T, 4, 1>>
			{

				__MYE_MATH_INLINE static Matrix<T, 4, 1> Tangent(const Matrix<T, 4, 1> & angles)
				{

					return Matrix<T, 4, 1>(TrigonometryFunctions<T>::Tangent(angles.x()),
										   TrigonometryFunctions<T>::Tangent(angles.y()),
										   TrigonometryFunctions<T>::Tangent(angles.z()),
										   TrigonometryFunctions<T>::Tangent(angles.w()));

				}

				__MYE_MATH_INLINE static Matrix<T, 4, 1> Arctangent(const Matrix<T, 4, 1> & x)
				{

					return Matrix<T, 4, 1>(TrigonometryFunctions<T>::Arctangent(x.x()),
										   TrigonometryFunctions<T>::Arctangent(x.y()),
										   TrigonometryFunctions<T>::Arctangent(x.z()),
										   TrigonometryFunctions<T>::Arctangent(x.w()));

				}

				__MYE_MATH_INLINE static Matrix<T, 4, 1> Cosine(const Matrix<T, 4, 1> & angles)
				{

					return Matrix<T, 4, 1>(TrigonometryFunctions<T>::Cosine(angles.x()),
										   TrigonometryFunctions<T>::Cosine(angles.y()),
										   TrigonometryFunctions<T>::Cosine(angles.z()),
										   TrigonometryFunctions<T>::Cosine(angles.w()));

				}

				__MYE_MATH_INLINE static Matrix<T, 4, 1> Arccosine(T x)
				{

					return Matrix<T, 4, 1>(TrigonometryFunctions<T>::Arccosine(x.x()),
										   TrigonometryFunctions<T>::Arccosine(x.y()),
										   TrigonometryFunctions<T>::Arccosine(x.z()),
										   TrigonometryFunctions<T>::Arccosine(x.w()));

				}

				__MYE_MATH_INLINE static Matrix<T, 4, 1> Sine(const Matrix<T, 4, 1> & angles)
				{

					return Matrix<T, 4, 1>(TrigonometryFunctions<T>::Sine(angles.x()),
										   TrigonometryFunctions<T>::Sine(angles.y()),
										   TrigonometryFunctions<T>::Sine(angles.z()),
										   TrigonometryFunctions<T>::Sine(angles.w()));

				}

				__MYE_MATH_INLINE static T Arcsine(const Matrix<T, 4, 1> & x)
				{

					return Matrix<T, 4, 1>(TrigonometryFunctions<T>::Arcsine(x.x()),
										   TrigonometryFunctions<T>::Arcsine(x.y()),
										   TrigonometryFunctions<T>::Arcsine(x.z()),
										   TrigonometryFunctions<T>::Arcsine(x.w()));

				}

			};

			template <typename T>
			struct TrigonometryFunctions<const T> : TrigonometryFunctions<T> { };

			template <typename T>
			struct TrigonometryFunctions<T &> : TrigonometryFunctions<T> { };

		}

	}

}