#pragma once

#define __MYE_MATH_FORCE_INLINE __forceinline
#define __MYE_MATH_INLINE       __MYE_MATH_FORCE_INLINE

#include "SSE.h"

namespace mye
{

	namespace math
	{

		typedef float Real;

		template <typename T, int ROWS, int COLS>
		class Matrix;

		typedef Matrix<float,        2, 2> __MYE_MATH_SSE_ALIGNED(16) Matrix2f;
		typedef Matrix<double,       2, 2> Matrix2d;
		typedef Matrix<int,          2, 2> Matrix2i;
		typedef Matrix<unsigned int, 2, 2> Matrix2u;
		typedef Matrix<Real,         2, 2> Matrix2;

		typedef Matrix<float,        3, 3> __MYE_MATH_SSE_ALIGNED(16) Matrix3f;
		typedef Matrix<double,       3, 3> Matrix3d;
		typedef Matrix<int,          3, 3> Matrix3i;
		typedef Matrix<unsigned int, 3, 3> Matrix3u;
		typedef Matrix<Real,         3, 3> Matrix3;

		typedef Matrix<float,        4, 4> __MYE_MATH_SSE_ALIGNED(16) Matrix4f;
		typedef Matrix<double,       4, 4> Matrix4d;
		typedef Matrix<int,          4, 4> Matrix4i;
		typedef Matrix<unsigned int, 4, 4> Matrix4u;
		typedef Matrix<Real,         4, 4> Matrix4;

		typedef Matrix<float,        2, 1> __MYE_MATH_SSE_ALIGNED(16) Vector2f;
		typedef Matrix<double,       2, 1> Vector2d;
		typedef Matrix<int,          2, 1> Vector2i;
		typedef Matrix<unsigned int, 2, 1> Vector2u;
		typedef Matrix<Real,         2, 1> Vector2;

		typedef Matrix<float,        3, 1> __MYE_MATH_SSE_ALIGNED(16) Vector3f;
		typedef Matrix<double,       3, 1> Vector3d;
		typedef Matrix<int,          3, 1> Vector3i;
		typedef Matrix<unsigned int, 3, 1> Vector3u;
		typedef Matrix<Real,         3, 1> Vector3;

		typedef Matrix<float,        4, 1> __MYE_MATH_SSE_ALIGNED(16) Vector4f;
		typedef Matrix<double,       4, 1> Vector4d;
		typedef Matrix<int,          4, 1> Vector4i;
		typedef Matrix<unsigned int, 4, 1> Vector4u;
		typedef Matrix<Real,         4, 1> Vector4;

		namespace detail
		{

			template <typename T, typename Enable = void>
			struct MathFunctions;

			template <typename T, typename Enable = void>
			struct TrigonometryFunctions;

		}

	}

}