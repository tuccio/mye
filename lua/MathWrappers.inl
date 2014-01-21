namespace mye
{

	namespace lua
	{

		/* Vectors */

		template <int N, typename T>
		mye::math::Matrix<T, N, 1> __vec_add(
			const mye::math::Matrix<T, N, 1> &a,
			const mye::math::Matrix<T, N, 1> &b)
		{
			return a + b;
		}

		template <int N, typename T>
		mye::math::Matrix<T, N, 1> __vec_add(
			const mye::math::Matrix<T, N, 1> &a,
			T b)
		{
			mye::math::Matrix<T, N, 1> v(b);
			return a + v;
		}

		template <int N, typename T>
		mye::math::Matrix<T, N, 1> __vec_sub(
			const mye::math::Matrix<T, N, 1> &a,
			const mye::math::Matrix<T, N, 1> &b)
		{
			return a - b;
		}

		template <int N, typename T>
		mye::math::Matrix<T, N, 1> __vec_sub(
			const mye::math::Matrix<T, N, 1> &a,
			T b)
		{
			mye::math::Matrix<T, N, 1> v(b);
			return a - v;
		}

		template <int N, typename T>
		mye::math::Matrix<T, N, 1> __vec_mul(
			const mye::math::Matrix<T, N, 1> &a,
			const mye::math::Matrix<T, N, 1> &b)
		{
			return a * b;
		}

		template <int N, typename T>
		mye::math::Matrix<T, N, 1> __vec_mul(
			const mye::math::Matrix<T, N, 1> &a,
			T b)
		{
			return a * b;
		}

		template <int N, typename T>
		mye::math::Matrix<T, N, 1> __vec_div(
			const mye::math::Matrix<T, N, 1> &a,
			const mye::math::Matrix<T, N, 1> &b)
		{
			return a / b;
		}

		template <int N, typename T>
		mye::math::Matrix<T, N, 1> __vec_div(
			const mye::math::Matrix<T, N, 1> &a,
			T b)
		{
			return a / b;
		}

		template <int N, typename T>
		std::string __vec_tostring(const mye::math::Matrix<T, N, 1> &v)
		{

			std::stringstream ss;

			ss << "[";

			for (int i = 0; i < N - 1; i++)
			{
				ss << v[i] << ", ";
			}

			ss << v[N - 1] << "]";

			return ss.str();

		}

		template <int N, typename T, int I>
		T __vec_get(const mye::math::Matrix<T, N, 1> &a)
		{
			return a[I];
		}

		template <int N, typename T, int I>
		void __vec_set(mye::math::Matrix<T, N, 1> &a, T b)
		{
			a[I] = b;
		}

		/* Quaternions */

		template <typename T>
		std::string __quat_tostring(const mye::math::Quaternion<T> &q)
		{

			std::stringstream ss;

			ss << "[" << q.w() << "; " <<
				q.x() << ", " << q.y() <<
				", " << q.z() << "]";

			return ss.str();

		}

		template <typename T, int I>
		T __quat_get(const mye::math::Quaternion<T> &q)
		{
			return q[I];
		}

		template <typename T, int I>
		void __quat_set(mye::math::Quaternion<T> &q, T b)
		{
			q[I] = b;
		}

	}

}
