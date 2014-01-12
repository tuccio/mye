namespace mye
{

	namespace lua
	{

		/* Vectors */

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_add(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			const Eigen::Matrix<T, N, 1, 0, N, 1> &b)
		{
			return a + b;
		}

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_add(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			T b)
		{
			Eigen::Matrix<T, N, 1, 0, N, 1> v;
			v.fill(b);
			return a + v;
		}

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_sub(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			const Eigen::Matrix<T, N, 1, 0, N, 1> &b)
		{
			return a - b;
		}

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_sub(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			T b)
		{
			Eigen::Matrix<T, N, 1, 0, N, 1> v;
			v.fill(b);
			return a - v;
		}

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_mul(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			const Eigen::Matrix<T, N, 1, 0, N, 1> &b)
		{
			return a.cwiseProduct(b);
		}

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_mul(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			T b)
		{
			return a * b;
		}

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_div(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			const Eigen::Matrix<T, N, 1, 0, N, 1> &b)
		{
			return a.cwiseProduct(b.cwiseInverse());
		}

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_div(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			T b)
		{
			return a / b;
		}

		template <int N, typename T>
		std::string __vec_tostring(const Eigen::Matrix<T, N, 1, 0, N, 1> &v)
		{

			static Eigen::IOFormat fmt(
				Eigen::FullPrecision,
				Eigen::DontAlignCols,
				", ",
				", ",
				"",
				"",
				"[",
				"]");

			std::stringstream ss;

			ss << v.format(fmt);

			return ss.str();

		}

		template <int N, typename T, int I>
		T __vec_get(const Eigen::Matrix<T, N, 1, 0, N, 1> &a)
		{
			return a[I];
		}

		template <int N, typename T, int I>
		void __vec_set(Eigen::Matrix<T, N, 1, 0, N, 1> &a, T b)
		{
			a[I] = b;
		}

		/* Quaternions */

		template <typename T>
		std::string __quat_tostring(const Eigen::Quaternion<T> &q)
		{

			std::stringstream ss;

			ss << "[" << q.w() << "; " <<
				q.x() << ", " << q.y() <<
				", " << q.z() << "]";

			return ss.str();

		}

		template <typename T, int I>
		T __quat_get(const Eigen::Quaternion<T> &q)
		{
			return q.coeffs().coeff(I);
		}

		template <typename T, int I>
		void __quat_set(Eigen::Quaternion<T> &q, T b)
		{
			q.coeffs().coeffRef(I) = b;
		}

	}

}
