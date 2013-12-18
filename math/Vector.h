#pragma once

#include <cstdlib>
#include <sstream>

namespace mye
{

	namespace math
	{

		template <int N, typename T>
		class Vector
		{

		public:

			/* Ctors */

			Vector(void);
			Vector(T *v);
			Vector(T x);
			Vector(T x, T y);
			Vector(T x, T y, T z);
			Vector(T x, T y, T z, T w);

			/* Algebraic operations */

			Vector<N, T> operator+ (const Vector<N, T> &v) const;
			Vector<N, T> operator- (const Vector<N, T> &v) const;
			Vector<N, T> operator* (const Vector<N, T> &v) const;
			Vector<N, T> operator/ (const Vector<N, T> &v) const;

			Vector<N, T>& operator+= (const Vector<N, T> &v);
			Vector<N, T>& operator-= (const Vector<N, T> &v);
			Vector<N, T>& operator*= (const Vector<N, T> &v);
			Vector<N, T>& operator/= (const Vector<N, T> &v);

			T Dot(const Vector<N, T> &v) const;
			Vector<N, T> Cross(const Vector<N, T> &v) const;

			float Length(void) const;

			/* Members access */

			inline const T& operator[] (int i) const;
			inline T& operator[] (int i);

		private:

			T _v[N];

		};

		/* Typedefs */

		typedef Vector<2, float> Vector2f;
		typedef Vector<2, int> Vector2i;
		typedef Vector<2, unsigned int> Vector2u;

		typedef Vector<3, float> Vector3f;
		typedef Vector<3, int> Vector3i;
		typedef Vector<3, unsigned int> Vector3u;

		typedef Vector<4, float> Vector4f;
		typedef Vector<4, int> Vector4i;
		typedef Vector<4, unsigned int> Vector4u;

		/* Implementation */

		#include "Vector.inl"

	}

}

#include "Vector3.h"