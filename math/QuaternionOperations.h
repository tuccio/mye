#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		inline Matrix<T, 3, 3> RotationMatrix3(const Quaternion<T> &q);

		template <typename T>
		inline Matrix<T, 4, 4> RotationMatrix4(const Quaternion<T> &q);

	}

}

#include "QuaternionOperations.inl"