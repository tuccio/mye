namespace mye
{

	namespace lua
	{

		/* Implementation */

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

		template <typename T>
		T __vec3_getx(const Eigen::Matrix<T, 3, 1, 0, 3, 1> &a)
		{
			return a.x();
		}

		template <typename T>
		void __vec3_setx(Eigen::Matrix<T, 3, 1, 0, 3, 1> &a, T b)
		{
			a.x() = b;
		}

		template <typename T>
		T __vec3_gety(const Eigen::Matrix<T, 3, 1, 0, 3, 1> &a)
		{
			return a.y();
		}

		template <typename T>
		void __vec3_sety(Eigen::Matrix<T, 3, 1, 0, 3, 1> &a, T b)
		{
			a.y() = b;
		}

		template <typename T>
		T __vec3_getz(const Eigen::Matrix<T, 3, 1, 0, 3, 1> &a)
		{
			return a.z();
		}

		template <typename T>
		void __vec3_setz(Eigen::Matrix<T, 3, 1, 0, 3, 1> &a, T b)
		{
			a.z() = b;
		}

	}

}
