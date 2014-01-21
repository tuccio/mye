#pragma once

namespace mye
{

	namespace math
	{


		template <typename T, int ROWS, int COLS>
		class Matrix;

		typedef Matrix<double, 3, 3> Matrix3d;
		typedef Matrix<float, 3, 3> Matrix3f;
		typedef Matrix<int, 3, 3> Matrix3i;
		typedef Matrix<unsigned int, 3, 3> Matrix3u;

		typedef Matrix<double, 4, 4> Matrix4d;
		typedef Matrix<float, 4, 4> Matrix4f;
		typedef Matrix<int, 4, 4> Matrix4i;
		typedef Matrix<unsigned int, 4, 4> Matrix4u;

		typedef Matrix<double, 2, 1> Vector2d;
		typedef Matrix<float, 2, 1> Vector2f;
		typedef Matrix<int, 2, 1> Vector2i;
		typedef Matrix<unsigned int, 2, 1> Vector2u;

		typedef Matrix<double, 3, 1> Vector3d;
		typedef Matrix<float, 3, 1> Vector3f;
		typedef Matrix<int, 3, 1> Vector3i;
		typedef Matrix<unsigned int, 3, 1> Vector3u;

		typedef Matrix<double, 4, 1> Vector4d;
		typedef Matrix<float, 4, 1> Vector4f;
		typedef Matrix<int, 4, 1> Vector4i;
		typedef Matrix<unsigned int, 4, 1> Vector4u;

		template <typename T>
		class Quaternion;

		typedef Quaternion<float> Quaternionf;
		typedef Quaternion<double> Quaterniond;

	}

}