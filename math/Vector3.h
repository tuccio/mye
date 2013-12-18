#pragma once

#include <boost/ref.hpp>

namespace mye
{

	namespace math
	{

		template <typename T>
		class Vector<3, T>
		{

		public:

			/* Ctors */

			Vector(T *v);
			Vector(T x = 0);
			Vector(T x, T y);
			Vector(T x, T y, T z);

			/* Algebraic operations */

			Vector<3, T> operator+ (const Vector<3, T> &v) const;
			Vector<3, T> operator- (const Vector<3, T> &v) const;
			Vector<3, T> operator* (const Vector<3, T> &v) const;
			Vector<3, T> operator/ (const Vector<3, T> &v) const;

			Vector<3, T>& operator+= (const Vector<3, T> &v);
			Vector<3, T>& operator-= (const Vector<3, T> &v);
			Vector<3, T>& operator*= (const Vector<3, T> &v);
			Vector<3, T>& operator/= (const Vector<3, T> &v);

			T Dot(const Vector<3, T> &v) const;
			Vector<3, T> Cross(const Vector<3, T> &v) const;

			float Length(void) const;

			/* Members access */

			inline const T& operator[] (int i) const;
			inline T& operator[] (int i);

			inline Vector<3, T>& operator= (const Vector<3, T>& v);

			inline T& x(void);
			inline T& y(void);
			inline T& z(void);

			inline T GetX(void) const;
			inline void SetX(float x);

			inline T GetY(void) const;
			inline void SetY(float y);

			inline T GetZ(void) const;
			inline void SetZ(float z);

		private:

			T _v[3];

		};

		/* Implementation */

		#include "Vector3.inl"

	}

}
