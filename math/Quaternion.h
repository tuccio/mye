#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Quaterniont
		{

		public:

			Quaterniont(void);
			Quaterniont(T w, T x, T y, T z);
			Quaterniont(const Matrix<T, 3, 1> &axis, T angle);
			Quaterniont(const Matrix<T, 3, 3> &A);

			inline Quaterniont Inverse(void) const;
			inline Quaterniont Conjugate(void) const;

			inline Matrix<T, 3, 1> Rotate(const Matrix<T, 3, 1> &p) const;

			inline T Norm(void) const;
			inline Quaterniont& Normalize(void);
			inline Quaterniont Normalized(void) const;

			inline Quaterniont operator+ (const Quaterniont &q) const;
			inline Quaterniont operator- (const Quaterniont &q) const;
			inline Quaterniont operator* (const Quaterniont &q) const;

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