namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 3, 3> RotationMatrix3(const Quaternion<T> &q)
		{

			Matrix<T, 3, 3> r;

			float twoXSquared = 2 * q.x() * q.x();
			float twoYSquared = 2 * q.y() * q.y();
			float twoZSquared = 2 * q.z() * q.z();
			float twoXY       = 2 * q.x() * q.y();
			float twoWZ       = 2 * q.w() * q.z();
			float twoXZ       = 2 * q.x() * q.z();
			float twoWY       = 2 * q.w() * q.y();
			float twoYZ       = 2 * q.y() * q.z();
			float twoWX       = 2 * q.w() * q.x();

			r(0, 0) = T(1) - twoYSquared - twoZSquared;
			r(0, 1) = twoXY - twoWZ;
			r(0, 2) = twoXZ + twoWY;

			r(1, 0) = twoXY + twoWZ;
			r(1, 1) = T(1) - twoXSquared - twoZSquared;
			r(1, 2) = twoYZ - twoWX;

			r(2, 0) = twoXZ - twoWY;
			r(2, 1) = twoYZ + twoWX;
			r(2, 2) = T(1) - twoXSquared - twoYSquared;

			return r;

		}

		template <typename T>
		Matrix<T, 4, 4> RotationMatrix4(const Quaternion<T> &q)
		{

			Matrix<T, 4, 4> r;

			float twoXSquared = 2 * q.x() * q.x();
			float twoYSquared = 2 * q.y() * q.y();
			float twoZSquared = 2 * q.z() * q.z();
			float twoXY       = 2 * q.x() * q.y();
			float twoWZ       = 2 * q.w() * q.z();
			float twoXZ       = 2 * q.x() * q.z();
			float twoWY       = 2 * q.w() * q.y();
			float twoYZ       = 2 * q.y() * q.z();
			float twoWX       = 2 * q.w() * q.x();

			r(0, 0) = T(1) - twoYSquared - twoZSquared;
			r(0, 1) = twoXY - twoWZ;
			r(0, 2) = twoXZ + twoWY;
			r(0, 3) = T(0);

			r(1, 0) = twoXY + twoWZ;
			r(1, 1) = T(1) - twoXSquared - twoZSquared;
			r(1, 2) = twoYZ - twoWX;
			r(1, 3) = T(0);

			r(2, 0) = twoXZ - twoWY;
			r(2, 1) = twoYZ + twoWX;
			r(2, 2) = T(1) - twoXSquared - twoYSquared;
			r(2, 3) = T(0);

			r(3, 0) = T(0);
			r(3, 1) = T(0);
			r(3, 2) = T(0);
			r(3, 3) = T(1);

			return r;

		}

	}

}