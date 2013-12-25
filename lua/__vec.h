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

		template <int N, typename T, int I>
		T __vec_get(const Eigen::Matrix<T, N, 1, 0, N, 1> &a);

	}

}

#include "__vec.inl"