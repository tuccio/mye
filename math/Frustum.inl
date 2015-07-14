#define __MYE_MATH_FRUSTUM_NEAR   m_planes[static_cast<int>(FrustumPlane::NEAR_PLANE)]
#define __MYE_MATH_FRUSTUM_FAR    m_planes[static_cast<int>(FrustumPlane::FAR_PLANE)]
#define __MYE_MATH_FRUSTUM_LEFT   m_planes[static_cast<int>(FrustumPlane::LEFT_PLANE)]
#define __MYE_MATH_FRUSTUM_RIGHT  m_planes[static_cast<int>(FrustumPlane::RIGHT_PLANE)]
#define __MYE_MATH_FRUSTUM_TOP    m_planes[static_cast<int>(FrustumPlane::TOP_PLANE)]
#define __MYE_MATH_FRUSTUM_BOTTOM m_planes[static_cast<int>(FrustumPlane::BOTTOM_PLANE)]

namespace mye
{

	namespace math
	{

		template <typename T>
		FrustumTempl<T>::FrustumTempl(void)
		{
		}

		template <typename T>
		FrustumTempl<T>::FrustumTempl(const Matrix<T, 3, 1> & origin,
		                              const Matrix<T, 3, 1> & forward,
		                              const Matrix<T, 3, 1> & up,
		                              const Matrix<T, 3, 1> & right,
		                              T nearPlaneDistance,
		                              T farPlaneDistance,
		                              T fovX,
		                              T fovY)
		{

			/* Construct planes */

			Matrix<T, 2, 1> tgHalfFov = Abs(Tangent(Radians(Matrix<T, 2, 1>(fovX, fovY) * T(0.5))));

			Matrix<T, 2, 1> halfFarSize = tgHalfFov * farPlaneDistance;

			Matrix<T, 3, 1> farHalfHeightVector = halfFarSize.yyy() * up;
			Matrix<T, 3, 1> farHalfWidthVector  = halfFarSize.xxx() * right;

			Matrix<T, 3, 1> nearPlaneCenter = origin + nearPlaneDistance * forward;
			Matrix<T, 3, 1> farPlaneCenter  = origin + farPlaneDistance  * forward;

			Matrix<T, 3, 1> topLeftFarCorner    = farPlaneCenter - farHalfWidthVector + farHalfHeightVector;
			Matrix<T, 3, 1> bottomLeftFarCorner = farPlaneCenter - farHalfWidthVector - farHalfHeightVector;
			Matrix<T, 3, 1> topRightFarCorner   = farPlaneCenter + farHalfWidthVector + farHalfHeightVector;

			Matrix<T, 3, 1> originTopLeftVector = topLeftFarCorner - origin;
			Matrix<T, 3, 1> topBottomVector     = topLeftFarCorner - bottomLeftFarCorner;
			Matrix<T, 3, 1> rightLeftVector     = topRightFarCorner - topLeftFarCorner;

			Matrix<T, 3, 1> leftNormal   = originTopLeftVector.Cross(topBottomVector).Normalize();
			Matrix<T, 3, 1> rightNormal  = leftNormal.Reflect(right);
			Matrix<T, 3, 1> topNormal    = originTopLeftVector.Cross(rightLeftVector).Normalize();
			Matrix<T, 3, 1> bottomNormal = topNormal.Reflect(up);

			__MYE_MATH_FRUSTUM_NEAR   = PlaneTempl<T>(nearPlaneCenter, forward.Reflect(forward));
			__MYE_MATH_FRUSTUM_FAR    = PlaneTempl<T>(farPlaneCenter,  forward);
			__MYE_MATH_FRUSTUM_LEFT   = PlaneTempl<T>(origin,          leftNormal);
			__MYE_MATH_FRUSTUM_RIGHT  = PlaneTempl<T>(origin,          rightNormal);
			__MYE_MATH_FRUSTUM_TOP    = PlaneTempl<T>(origin,          topNormal);
			__MYE_MATH_FRUSTUM_BOTTOM = PlaneTempl<T>(origin,          bottomNormal);

		}

		template <typename T>
		std::array<Matrix<T, 3, 1>, 8> FrustumTempl<T>::GetCorners(void) const
		{

			std::array<Matrix<T, 3, 1>, 8> corners;

			/* Left, bottom, near */

			{

				Matrix<T, 3, 3> A;
				Matrix<T, 3, 1> b;

				A.SetRow(0, __MYE_MATH_FRUSTUM_LEFT.Normal());
				A.SetRow(1, __MYE_MATH_FRUSTUM_BOTTOM.Normal());
				A.SetRow(2, __MYE_MATH_FRUSTUM_NEAR.Normal());

				b.x() = -__MYE_MATH_FRUSTUM_LEFT.Coefficient();
				b.y() = -__MYE_MATH_FRUSTUM_BOTTOM.Coefficient();
				b.z() = -__MYE_MATH_FRUSTUM_NEAR.Coefficient();

				corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_NEAR)] = Cramer(A, b);

			}

			/* Right, bottom, near */

			{

				Matrix<T, 3, 3> A;
				Matrix<T, 3, 1> b;

				A.SetRow(0, __MYE_MATH_FRUSTUM_RIGHT.Normal());
				A.SetRow(1, __MYE_MATH_FRUSTUM_BOTTOM.Normal());
				A.SetRow(2, __MYE_MATH_FRUSTUM_NEAR.Normal());

				b.x() = -__MYE_MATH_FRUSTUM_RIGHT.Coefficient();
				b.y() = -__MYE_MATH_FRUSTUM_BOTTOM.Coefficient();
				b.z() = -__MYE_MATH_FRUSTUM_NEAR.Coefficient();

				corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_NEAR)] = Cramer(A, b);

			}

			/* Right, top, near */

			{

				Matrix<T, 3, 3> A;
				Matrix<T, 3, 1> b;

				A.SetRow(0, __MYE_MATH_FRUSTUM_RIGHT.Normal());
				A.SetRow(1, __MYE_MATH_FRUSTUM_TOP.Normal());
				A.SetRow(2, __MYE_MATH_FRUSTUM_NEAR.Normal());

				b.x() = -__MYE_MATH_FRUSTUM_RIGHT.Coefficient();
				b.y() = -__MYE_MATH_FRUSTUM_TOP.Coefficient();
				b.z() = -__MYE_MATH_FRUSTUM_NEAR.Coefficient();

				corners[static_cast<int>(FrustumCorners::RIGHT_TOP_NEAR)] = Cramer(A, b);

			}

			/* Left, top, near */

			{

				Matrix<T, 3, 3> A;
				Matrix<T, 3, 1> b;

				A.SetRow(0, __MYE_MATH_FRUSTUM_LEFT.Normal());
				A.SetRow(1, __MYE_MATH_FRUSTUM_TOP.Normal());
				A.SetRow(2, __MYE_MATH_FRUSTUM_NEAR.Normal());

				b.x() = -__MYE_MATH_FRUSTUM_LEFT.Coefficient();
				b.y() = -__MYE_MATH_FRUSTUM_TOP.Coefficient();
				b.z() = -__MYE_MATH_FRUSTUM_NEAR.Coefficient();

				corners[static_cast<int>(FrustumCorners::LEFT_TOP_NEAR)] = Cramer(A, b);

			}

			/* Left, bottom, far */

			{

				Matrix<T, 3, 3> A;
				Matrix<T, 3, 1> b;

				A.SetRow(0, __MYE_MATH_FRUSTUM_LEFT.Normal());
				A.SetRow(1, __MYE_MATH_FRUSTUM_BOTTOM.Normal());
				A.SetRow(2, __MYE_MATH_FRUSTUM_FAR.Normal());

				b.x() = -__MYE_MATH_FRUSTUM_LEFT.Coefficient();
				b.y() = -__MYE_MATH_FRUSTUM_BOTTOM.Coefficient();
				b.z() = -__MYE_MATH_FRUSTUM_FAR.Coefficient();

				corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)] = Cramer(A, b);

			}

			/* Right, bottom, far */

			{

				Matrix<T, 3, 3> A;
				Matrix<T, 3, 1> b;

				A.SetRow(0, __MYE_MATH_FRUSTUM_RIGHT.Normal());
				A.SetRow(1, __MYE_MATH_FRUSTUM_BOTTOM.Normal());
				A.SetRow(2, __MYE_MATH_FRUSTUM_FAR.Normal());

				b.x() = -__MYE_MATH_FRUSTUM_RIGHT.Coefficient();
				b.y() = -__MYE_MATH_FRUSTUM_BOTTOM.Coefficient();
				b.z() = -__MYE_MATH_FRUSTUM_FAR.Coefficient();

				corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)] = Cramer(A, b);

			}

			/* Right, top, far */

			{

				Matrix<T, 3, 3> A;
				Matrix<T, 3, 1> b;

				A.SetRow(0, __MYE_MATH_FRUSTUM_RIGHT.Normal());
				A.SetRow(1, __MYE_MATH_FRUSTUM_TOP.Normal());
				A.SetRow(2, __MYE_MATH_FRUSTUM_FAR.Normal());

				b.x() = -__MYE_MATH_FRUSTUM_RIGHT.Coefficient();
				b.y() = -__MYE_MATH_FRUSTUM_TOP.Coefficient();
				b.z() = -__MYE_MATH_FRUSTUM_FAR.Coefficient();

				corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)] = Cramer(A, b);

			}

			/* Left, top, far */

			{

				Matrix<T, 3, 3> A;
				Matrix<T, 3, 1> b;

				A.SetRow(0, __MYE_MATH_FRUSTUM_LEFT.Normal());
				A.SetRow(1, __MYE_MATH_FRUSTUM_TOP.Normal());
				A.SetRow(2, __MYE_MATH_FRUSTUM_FAR.Normal());

				b.x() = -__MYE_MATH_FRUSTUM_LEFT.Coefficient();
				b.y() = -__MYE_MATH_FRUSTUM_TOP.Coefficient();
				b.z() = -__MYE_MATH_FRUSTUM_FAR.Coefficient();

				corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)] = Cramer(A, b);

			}

			return corners;

		}

		template <typename T>
		VolumeSide FrustumTempl<T>::Side(const Matrix<T, 3, 1> & x) const
		{

			PlaneSide nearSide = __MYE_MATH_FRUSTUM_NEAR.Side(x);

			if (nearSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide farSide = __MYE_MATH_FRUSTUM_FAR.Side(x);

			if (farSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide leftSide = __MYE_MATH_FRUSTUM_LEFT.Side(x);

			if (leftSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide rightSide = __MYE_MATH_FRUSTUM_RIGHT.Side(x);

			if (rightSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide topSide = __MYE_MATH_FRUSTUM_TOP.Side(x);

			if (topSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide bottomSide = __MYE_MATH_FRUSTUM_BOTTOM.Side(x);

			if (bottomSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			if (topSide    == PlaneSide::INSIDE ||
				bottomSide == PlaneSide::INSIDE ||
				rightSide  == PlaneSide::INSIDE ||
				leftSide   == PlaneSide::INSIDE ||
				nearSide   == PlaneSide::INSIDE ||
				farSide    == PlaneSide::INSIDE)
			{
				return VolumeSide::INTERSECT;
			}
			else
			{
				return VolumeSide::INSIDE;
			}

		}

		template <typename T>
		const PlaneTempl<T> & FrustumTempl<T>::GetPlane(FrustumPlane plane) const
		{
			return m_planes[static_cast<int>(plane)];
		}

		/*template <typename T>
		void FrustumTempl<T>::TransformAffine(Volume & volume,
											  const Matrix<T, 4, 4> & t) const
		{

			FrustumTempl<T> & f = static_cast<FrustumTempl<T> &>(volume);

			for (int fp = static_cast<int>(FrustumPlane::FIRST); fp <= static_cast<int>(FrustumPlane::LAST); fp++)
			{
				f.m_planes[fp] = m_planes[fp].Transform(t).Normalize();
			}

		}*/

		template <typename T>
		void FrustumTempl<T>::Split(T ratio, FrustumTempl<T> & f1, FrustumTempl<T> & f2) const
		{

			Matrix<T, 4, 1> farPlane = __MYE_MATH_FRUSTUM_FAR.Parameters();
			Matrix<T, 3, 1> farPoint = - farPlane.xyz() * farPlane.www();

			T distance = ratio * __MYE_MATH_FRUSTUM_NEAR.DistanceNormalized(farPoint);

			Plane np = __MYE_MATH_FRUSTUM_NEAR.TranslateAlongNormal(distance);

			f1.__MYE_MATH_FRUSTUM_NEAR   = __MYE_MATH_FRUSTUM_NEAR;
			f1.__MYE_MATH_FRUSTUM_FAR    = np.Flip();
			f1.__MYE_MATH_FRUSTUM_LEFT   = __MYE_MATH_FRUSTUM_LEFT;
			f1.__MYE_MATH_FRUSTUM_RIGHT  = __MYE_MATH_FRUSTUM_RIGHT;
			f1.__MYE_MATH_FRUSTUM_TOP    = __MYE_MATH_FRUSTUM_TOP;
			f1.__MYE_MATH_FRUSTUM_BOTTOM = __MYE_MATH_FRUSTUM_BOTTOM;

			f2.__MYE_MATH_FRUSTUM_NEAR   = np;
			f2.__MYE_MATH_FRUSTUM_FAR    = __MYE_MATH_FRUSTUM_FAR;
			f2.__MYE_MATH_FRUSTUM_LEFT   = __MYE_MATH_FRUSTUM_LEFT;
			f2.__MYE_MATH_FRUSTUM_RIGHT  = __MYE_MATH_FRUSTUM_RIGHT;
			f2.__MYE_MATH_FRUSTUM_TOP    = __MYE_MATH_FRUSTUM_TOP;
			f2.__MYE_MATH_FRUSTUM_BOTTOM = __MYE_MATH_FRUSTUM_BOTTOM;


		}

	}

}

#undef __MYE_MATH_FRUSTUM_NEAR   
#undef __MYE_MATH_FRUSTUM_FAR    
#undef __MYE_MATH_FRUSTUM_LEFT   
#undef __MYE_MATH_FRUSTUM_RIGHT  
#undef __MYE_MATH_FRUSTUM_TOP    
#undef __MYE_MATH_FRUSTUM_BOTTOM