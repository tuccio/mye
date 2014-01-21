#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Quaternion
		{

		public:

			Quaternion(void);
			Quaternion(T w, T x, T y, T z);
			Quaternion(const Matrix<T, 3, 1> &axis, T angle);
			Quaternion(const Matrix<T, 3, 3> &A);

			inline Quaternion Inverse(void) const;
			inline Quaternion Conjugate(void) const;

			inline Matrix<T, 3, 1> Rotate(const Matrix<T, 3, 1> &p) const;

			inline T Norm(void) const;
			inline Quaternion& Normalize(void);
			inline Quaternion Normalized(void) const;

			inline Quaternion operator+ (const Quaternion &q) const;
			inline Quaternion operator- (const Quaternion &q) const;
			inline Quaternion operator* (const Quaternion &q) const;

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