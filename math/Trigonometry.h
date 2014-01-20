#include <cmath>
#include <mye/core/Singleton.h>

#define MYE_GET_CONSTANTS(__VAR, __TYPE) \
	Constants<__TYPE> *constants = Constants<__TYPE>::GetSingletonPointer();\
	if (!constants)\
	{\
		constants = new Constants<__TYPE>();\
	}

namespace mye
{

	namespace math
	{

		template <typename T>
		class Constants :
			public mye::core::Singleton<Constants<T>>
		{

		public:

			Constants(void)
			{
				m_pi = ::atan(T(1)) * T(4);
				m_twopi = m_pi * T(2);
			}

			inline T Pi(void)
			{
				return m_pi;
			}

			inline T TwoPi(void)
			{
				return m_pi;
			}

		private:

			T m_pi;
			T m_twopi;

		};

		template <typename T>
		inline T Radians(T degAngle)
		{
			MYE_GET_CONSTANTS(constants, T);
			return degAngle * constants->Pi() / 180.0f;
		}

		template <typename T>
		inline T Degrees(T radAngle)
		{
			MYE_GET_CONSTANTS(constants, T);
			return radAngle * 180.f / constants->Pi();
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