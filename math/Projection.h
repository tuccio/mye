namespace mye
{

	namespace math
	{


		template <typename T = mye::math::Real>
		Matrix<T, 4, 4> OrthographicProjectionD3DLH(T w, T h, T n, T f)
		{

			T iWidth  = T(1) / w;
			T iHeight = T(1) / h;
			T iDepth  = T(1) / (f - n);

			Matrix<T, 4, 4> m(1);

			m.m00() = T(2) * iWidth;
			m.m11() = T(2) * iHeight;
			m.m22() = T(1) * iDepth;

			m.m23() = - n * iDepth;

			return m;

		}

		template <typename T = mye::math::Real>
		Matrix<T, 4, 4> OrthographicProjectionD3DLH(T l, T r, T b, T t, T n, T f)
		{

			T iRmL = T(1) / (r - l);
			T iTmB = T(1) / (t - b);
			T iFmN = T(1) / (f - n);

			Matrix<T, 4, 4> m(1);

			m.m00() = T(2) * iRmL;
			m.m03() = - (l + r) * iRmL;

			m.m11() = T(2) * iTmB;
			m.m13() = - (t + b) * iTmB;

			m.m22() = T(1) * iFmN;
			m.m23() = - n * iFmN;

			return m;

		}

	}

}