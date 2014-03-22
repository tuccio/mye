namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 3, 3> RotationMatrix3(const Quaterniont<T> &q)
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

			r.m00() = T(1) - twoYSquared - twoZSquared;
			r.m01() = twoXY - twoWZ;
			r.m02() = twoXZ + twoWY;

			r.m10() = twoXY + twoWZ;
			r.m11() = T(1) - twoXSquared - twoZSquared;
			r.m12() = twoYZ - twoWX;

			r.m20() = twoXZ - twoWY;
			r.m21() = twoYZ + twoWX;
			r.m22() = T(1) - twoXSquared - twoYSquared;

			return r;

		}

		template <typename T>
		Matrix<T, 4, 4> RotationMatrix4(const Quaterniont<T> &q)
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

			r.m00() = T(1) - twoYSquared - twoZSquared;
			r.m01() = twoXY - twoWZ;
			r.m02() = twoXZ + twoWY;
			r.m03() = T(0);

			r.m10() = twoXY + twoWZ;
			r.m11() = T(1) - twoXSquared - twoZSquared;
			r.m12() = twoYZ - twoWX;
			r.m13() = T(0);

			r.m20() = twoXZ - twoWY;
			r.m21() = twoYZ + twoWX;
			r.m22() = T(1) - twoXSquared - twoYSquared;
			r.m23() = T(0);

			r.m30() = T(0);
			r.m31() = T(0);
			r.m32() = T(0);
			r.m33() = T(1);

			return r;

		}

	}

}