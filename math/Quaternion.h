#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class QuaternionTempl
		{

		public:

			QuaternionTempl(void);
			QuaternionTempl(T w, T x, T y, T z);
			QuaternionTempl(const Matrix<T, 3, 1> & axis, T angle);
			QuaternionTempl(const Matrix<T, 3, 3> & A);

			__MYE_MATH_INLINE QuaternionTempl Inverse(void) const;
			__MYE_MATH_INLINE QuaternionTempl Conjugate(void) const;

			__MYE_MATH_INLINE Matrix<T, 3, 1> Rotate(const Matrix<T, 3, 1> & p) const;

			__MYE_MATH_INLINE T Norm(void) const;
			__MYE_MATH_INLINE QuaternionTempl Normalize(void) const;

			__MYE_MATH_INLINE QuaternionTempl operator+ (const QuaternionTempl & q) const;
			__MYE_MATH_INLINE QuaternionTempl operator- (const QuaternionTempl & q) const;
			__MYE_MATH_INLINE QuaternionTempl operator* (const QuaternionTempl & q) const;

			__MYE_MATH_INLINE T &       operator[] (int i);
			__MYE_MATH_INLINE const T & operator[] (int i) const;

			__MYE_MATH_INLINE T &       x(void);
			__MYE_MATH_INLINE const T & x(void) const;

			__MYE_MATH_INLINE T &       y(void);
			__MYE_MATH_INLINE const T & y(void) const;

			__MYE_MATH_INLINE T &       z(void);
			__MYE_MATH_INLINE const T & z(void) const;

			__MYE_MATH_INLINE T &       w(void);
			__MYE_MATH_INLINE const T & w(void) const;

		private:

			T m_data[4];

		};

	}

}

#include "Quaternion.inl"