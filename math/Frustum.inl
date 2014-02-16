namespace mye
{

	namespace math
	{

		template <typename T>
		Frustum<T>::Frustum(void) :
		Volume(VolumeType::FRUSTUM)
		{
		}

		template <typename T>
		Frustum<T>::Frustum(const Matrix<T, 3, 1> &origin,
			const Matrix<T, 3, 1>& forward,
			const Matrix<T, 3, 1>& up,
			const Matrix<T, 3, 1>& right,
			T nearPlaneDistance,
			T farPlaneDistance,
			T fovX,
			T fovY) :
		Volume(VolumeType::FRUSTUM)
		{

			/* Construct planes */

			T tgFovX = Abs(Tangent(Radians(fovX * 0.5f)));
			T tgFovY = Abs(Tangent(Radians(fovY * 0.5f)));

			T nearHeight = tgFovY * nearPlaneDistance;
			T nearWidth  = tgFovX * nearPlaneDistance;

			Matrix<T, 3, 1> nearHeightVector = nearHeight * up;
			Matrix<T, 3, 1> nearWidthVector = nearWidth * right;

			Matrix<T, 3, 1> nearPlaneCenter = origin + nearPlaneDistance * forward;
			Matrix<T, 3, 1> farPlaneCenter  = origin + farPlaneDistance * forward;

			Matrix<T, 3, 1> tln = nearPlaneCenter - nearWidthVector + nearHeightVector;
			Matrix<T, 3, 1> bln = nearPlaneCenter - nearWidthVector - nearHeightVector;
			Matrix<T, 3, 1> trn = nearPlaneCenter + nearWidthVector + nearHeightVector;

 			Matrix<T, 3, 1> otl = tln - origin;

			Matrix<T, 3, 1> ln = otl.Cross(tln - bln).Normalize();
			Matrix<T, 3, 1> tn = otl.Cross(trn - tln).Normalize();

			m_planes[static_cast<int>(FrustumPlanes::NEAR_PLANE)]   = Plane<T>(nearPlaneCenter, forward.Reflect(forward));
			m_planes[static_cast<int>(FrustumPlanes::FAR_PLANE)]    = Plane<T>(farPlaneCenter, forward);
			m_planes[static_cast<int>(FrustumPlanes::LEFT_PLANE)]   = Plane<T>(origin, ln);
			m_planes[static_cast<int>(FrustumPlanes::RIGHT_PLANE)]  = Plane<T>(origin, ln.Reflect(right));
			m_planes[static_cast<int>(FrustumPlanes::TOP_PLANE)]    = Plane<T>(origin, tn);
			m_planes[static_cast<int>(FrustumPlanes::BOTTOM_PLANE)] = Plane<T>(origin, tn.Reflect(up));

		}

		template <typename T>
		std::vector<Matrix<T, 3, 1>> Frustum<T>::GetCorners(void) const
		{

			std::vector<Matrix<T, 3, 1>> corners(8);

			Matrix<T, 3, 3> A;
			Matrix<T, 3, 1> b;

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlanes::LEFT_PLANE)].Normal());
			A.SetRow(1, m_planes[static_cast<int>(FrustumPlanes::BOTTOM_PLANE)].Normal());
			A.SetRow(2, m_planes[static_cast<int>(FrustumPlanes::NEAR_PLANE)].Normal());

			b.x() = m_planes[static_cast<int>(FrustumPlanes::LEFT_PLANE)].Coefficient();
			b.y() = m_planes[static_cast<int>(FrustumPlanes::BOTTOM_PLANE)].Coefficient();
			b.z() = m_planes[static_cast<int>(FrustumPlanes::NEAR_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_NEAR)] = Cramer(A, b);

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlanes::RIGHT_PLANE)].Normal());
			b.x() = m_planes[static_cast<int>(FrustumPlanes::RIGHT_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_NEAR)] = Cramer(A, b);

			A.SetRow(1, m_planes[static_cast<int>(FrustumPlanes::TOP_PLANE)].Normal());
			b.y() = m_planes[static_cast<int>(FrustumPlanes::TOP_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_TOP_NEAR)] = Cramer(A, b);

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlanes::LEFT_PLANE)].Normal());
			b.x() = m_planes[static_cast<int>(FrustumPlanes::LEFT_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_TOP_NEAR)] = Cramer(A, b);

			A.SetRow(1, m_planes[static_cast<int>(FrustumPlanes::BOTTOM_PLANE)].Normal());
			b.y() = m_planes[static_cast<int>(FrustumPlanes::BOTTOM_PLANE)].Coefficient();

			A.SetRow(2, m_planes[static_cast<int>(FrustumPlanes::FAR_PLANE)].Normal());
			b.z() = m_planes[static_cast<int>(FrustumPlanes::FAR_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)] = Cramer(A, b);

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlanes::RIGHT_PLANE)].Normal());
			b.x() = m_planes[static_cast<int>(FrustumPlanes::RIGHT_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)] = Cramer(A, b);

			A.SetRow(1, m_planes[static_cast<int>(FrustumPlanes::TOP_PLANE)].Normal());
			b.y() = m_planes[static_cast<int>(FrustumPlanes::TOP_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)] = Cramer(A, b);

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlanes::LEFT_PLANE)].Normal());
			b.x() = m_planes[static_cast<int>(FrustumPlanes::LEFT_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)] = Cramer(A, b);

			return corners;

		}

		template <typename T>
		VolumeSide Frustum<T>::Side(const Matrix<T, 3, 1> &x) const
		{

			PlaneSide nearSide = m_plane[FrustumPlanes::NEAR_PLANE].Side(x);

			if (nearSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide farSide = m_plane[FrustumPlanes::FAR_PLANE].Side(x);

			if (farSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide leftSide = m_plane[FrustumPlanes::LEFT_PLANE].Side(x);

			if (leftSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide rightSide = m_plane[FrustumPlanes::RIGHT_PLANE].Side(x);

			if (rightSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide topSide = m_plane[FrustumPlanes::TOP_PLANE].Side(x);

			if (topSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide bottomSide = m_plane[FrustumPlanes::BOTTOM_PLANE].Side(x);

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
		const Plane<T>& Frustum<T>::GetPlane(FrustumPlanes plane) const
		{
			return m_planes[static_cast<int>(plane)];
		}

// 		template <typename T>
// 		const Plane<T>& Frustum<T>::Near(void) const
// 		{
// 			return m_near;
// 		}
// 
// 		template <typename T>
// 		const Plane<T>& Frustum<T>::Far(void) const
// 		{
// 			return m_far;
// 		}
// 
// 		template <typename T>
// 		const Plane<T>& Frustum<T>::Left(void) const
// 		{
// 			return m_left;
// 		}
// 
// 		template <typename T>
// 		const Plane<T>& Frustum<T>::Right(void) const
// 		{
// 			return m_right;
// 		}
// 
// 		template <typename T>
// 		const Plane<T>& Frustum<T>::Top(void) const
// 		{
// 			return m_top;
// 		}
// 
// 		template <typename T>
// 		const Plane<T>& Frustum<T>::Bottom(void) const
// 		{
// 			return m_bottom;
// 		}

	}

}