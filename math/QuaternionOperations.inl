namespace mye
{

	namespace math
	{

		template <typename T>
		Matrix<T, 3, 3> RotationMatrix3(const QuaternionTempl<T> &q)
		{

			Matrix<T, 3, 3> r;

			T twoXSquared = T(2) * q.x() * q.x();
			T twoYSquared = T(2) * q.y() * q.y();
			T twoZSquared = T(2) * q.z() * q.z();
			T twoXY = T(2) * q.x() * q.y();
			T twoWZ = T(2) * q.w() * q.z();
			T twoXZ = T(2) * q.x() * q.z();
			T twoWY = T(2) * q.w() * q.y();
			T twoYZ = T(2) * q.y() * q.z();
			T twoWX = T(2) * q.w() * q.x();

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
		Matrix<T, 4, 4> RotationMatrix4(const QuaternionTempl<T> &q)
		{

			Matrix<T, 4, 4> r;

			T twoXSquared = T(2) * q.x() * q.x();
			T twoYSquared = T(2) * q.y() * q.y();
			T twoZSquared = T(2) * q.z() * q.z();
			T twoXY = T(2) * q.x() * q.y();
			T twoWZ = T(2) * q.w() * q.z();
			T twoXZ = T(2) * q.x() * q.z();
			T twoWY = T(2) * q.w() * q.y();
			T twoYZ = T(2) * q.y() * q.z();
			T twoWX = T(2) * q.w() * q.x();

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

		template <typename T>
		Matrix<T, 4, 4> RotationTranslationMatrix4(const QuaternionTempl<T> &q, const Matrix<T, 3, 1> &t)
		{

			Matrix<T, 4, 4> r;

			T twoXSquared = T(2) * q.x() * q.x();
			T twoYSquared = T(2) * q.y() * q.y();
			T twoZSquared = T(2) * q.z() * q.z();
			T twoXY       = T(2) * q.x() * q.y();
			T twoWZ       = T(2) * q.w() * q.z();
			T twoXZ       = T(2) * q.x() * q.z();
			T twoWY       = T(2) * q.w() * q.y();
			T twoYZ       = T(2) * q.y() * q.z();
			T twoWX       = T(2) * q.w() * q.x();

			r.m00() = T(1) - twoYSquared - twoZSquared;
			r.m01() = twoXY - twoWZ;
			r.m02() = twoXZ + twoWY;
			r.m03() = t.x();

			r.m10() = twoXY + twoWZ;
			r.m11() = T(1) - twoXSquared - twoZSquared;
			r.m12() = twoYZ - twoWX;
			r.m13() = t.y();

			r.m20() = twoXZ - twoWY;
			r.m21() = twoYZ + twoWX;
			r.m22() = T(1) - twoXSquared - twoYSquared;
			r.m23() = t.z();

			r.m30() = T(0);
			r.m31() = T(0);
			r.m32() = T(0);
			r.m33() = T(1);

			return r;

		}

		template <typename T>
		Matrix<T, 3, 1> EulerAngles(const QuaternionTempl<T> &q)
		{

			T twoZSquared = T(2) * q.z() * q.z();

			return Matrix<T, 3, 1>(
				std::atan2(T(2) * q.y() * q.w() - T(2) * q.x() * q.z(), T(1) - T(2) * q.y() * q.y() - twoZSquared),
				std::asin (T(2) * q.x() * q.y() + T(2) * q.z() * q.w()),
				std::atan2(T(2) * q.x() * q.w() - T(2) * q.y() * q.z(), T(1) - T(2) * q.x() * q.x() - twoZSquared)
				);

		}

	}

}