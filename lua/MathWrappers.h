#pragma once

#include <mye/math/Math.h>

namespace mye
{

	namespace lua
	{

		/* Vectors */

		template <int N, typename T>
		inline mye::math::Matrix<T, N, 1> __vec_add(
			const mye::math::Matrix<T, N, 1> &a,
			const mye::math::Matrix<T, N, 1> &b);

		template <int N, typename T>
		inline mye::math::Matrix<T, N, 1> __vec_add(
			const mye::math::Matrix<T, N, 1> &a,
			T b);

		template <int N, typename T>
		inline mye::math::Matrix<T, N, 1> __vec_sub(
			const mye::math::Matrix<T, N, 1> &a,
			const mye::math::Matrix<T, N, 1> &b);

		template <int N, typename T>
		inline mye::math::Matrix<T, N, 1> __vec_sub(
			const mye::math::Matrix<T, N, 1> &a,
			T b);

		template <int N, typename T>
		inline mye::math::Matrix<T, N, 1> __vec_mul(
			const mye::math::Matrix<T, N, 1> &a,
			const mye::math::Matrix<T, N, 1> &b);

		template <int N, typename T>
		inline mye::math::Matrix<T, N, 1> __vec_mul(
			const mye::math::Matrix<T, N, 1> &a,
			T b);

		template <int N, typename T>
		inline mye::math::Matrix<T, N, 1> __vec_div(
			const mye::math::Matrix<T, N, 1> &a,
			const mye::math::Matrix<T, N, 1> &b);

		template <int N, typename T>
		inline mye::math::Matrix<T, N, 1> __vec_div(
			const mye::math::Matrix<T, N, 1> &a,
			T b);

		template <int N, typename T>
		inline mye::core::String __vec_tostring(const mye::math::Matrix<T, N, 1> &v);

		template <int N, typename T, int I>
		inline T __vec_get(const mye::math::Matrix<T, N, 1> &a);

		template <int N, typename T, int I>
		inline void __vec_set(mye::math::Matrix<T, N, 1> &a, T b);

		/* Quaternions */

		template <typename T>
		inline mye::core::String __quat_tostring(const mye::math::Quaterniont<T> &q);

		template <typename T, int I>
		inline T __quat_get(const mye::math::Quaterniont<T> &q);

		template <typename T, int I>
		inline void __quat_set(mye::math::Quaterniont<T> &q, T b);

	}

}

#include "MathWrappers.inl"