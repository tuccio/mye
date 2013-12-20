#pragma once

#include <Eigen/Eigen>

namespace mye
{

	namespace lua
	{

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_add(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			const Eigen::Matrix<T, N, 1, 0, N, 1> &b);

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_add(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			T b);

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_sub(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			const Eigen::Matrix<T, N, 1, 0, N, 1> &b);

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_sub(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			T b);

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_mul(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			const Eigen::Matrix<T, N, 1, 0, N, 1> &b);

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_mul(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			T b);

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_div(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			const Eigen::Matrix<T, N, 1, 0, N, 1> &b);

		template <int N, typename T>
		Eigen::Matrix<T, N, 1, 0, N, 1> __vec_div(
			const Eigen::Matrix<T, N, 1, 0, N, 1> &a,
			T b);

		template <int N, typename T>
		std::string __vec_tostring(const Eigen::Matrix<T, N, 1, 0, N, 1> &v);

		template <typename T>
		T __vec3_getx(const Eigen::Matrix<T, 3, 1, 0, 3, 1> &a);

		template <typename T>
		void __vec3_setx(Eigen::Matrix<T, 3, 1, 0, 3, 1> &a, T b);

		template <typename T>
		T __vec3_gety(const Eigen::Matrix<T, 3, 1, 0, 3, 1> &a);

		template <typename T>
		void __vec3_sety(Eigen::Matrix<T, 3, 1, 0, 3, 1> &a, T b);

		template <typename T>
		T __vec3_getz(const Eigen::Matrix<T, 3, 1, 0, 3, 1> &a);

		template <typename T>
		void __vec3_setz(Eigen::Matrix<T, 3, 1, 0, 3, 1> &a, T b);

	}

}

#include "__vec.inl"