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
			QuaternionTempl(const Matrix<T, 3, 1> &axis, T angle);
			QuaternionTempl(const Matrix<T, 3, 3> &A);

			inline QuaternionTempl Inverse(void) const;
			inline QuaternionTempl Conjugate(void) const;

			inline Matrix<T, 3, 1> Rotate(const Matrix<T, 3, 1> &p) const;

			inline T Norm(void) const;
			inline QuaternionTempl Normalize(void) const;

			inline QuaternionTempl operator+ (const QuaternionTempl &q) const;
			inline QuaternionTempl operator- (const QuaternionTempl &q) const;
			inline QuaternionTempl operator* (const QuaternionTempl &q) const;

			inline T& operator[] (int i);
			inline const T& operator[] (int i) const;

			inline T& x(void);
			inline const T& x(void) const;

			inline T& y(void);
			inline const T& y(void) const;

			inline T& z(void);
			inline const T& z(void) const;

			inline T& w(void);
			inline const T& w(void) const;

		private:

			T m_data[4];

		};

	}

}

#include "Quaternion.inl"