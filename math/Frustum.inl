namespace mye
{

	namespace math
	{

		template <typename T>
		FrustumTempl<T>::FrustumTempl(void) :
		Volume(VolumeType::FRUSTUM)
		{
		}

		template <typename T>
		FrustumTempl<T>::FrustumTempl(const Matrix<T, 3, 1> &origin,
			const Matrix<T, 3, 1> & forward,
			const Matrix<T, 3, 1> & up,
			const Matrix<T, 3, 1> & right,
			T nearPlaneDistance,
			T farPlaneDistance,
			T fovX,
			T fovY) :
		Volume(VolumeType::FRUSTUM)
		{

			/* Construct planes */

			Matrix<T, 2, 1> tgFov    = Abs(Tangent(Radians(Matrix<T, 2, 1>(fovX, fovY) * 0.5f)));
			Matrix<T, 2, 1> nearSize = tgFov * Matrix<T, 2, 1>(nearPlaneDistance);

			Matrix<T, 3, 1> nearHeightVector = nearSize.y() * up;
			Matrix<T, 3, 1> nearWidthVector  = nearSize.x() * right;

			Matrix<T, 3, 1> nearPlaneCenter = origin + nearPlaneDistance * forward;
			Matrix<T, 3, 1> farPlaneCenter  = origin + farPlaneDistance * forward;

			Matrix<T, 3, 1> tln = nearPlaneCenter - nearWidthVector + nearHeightVector;
			Matrix<T, 3, 1> bln = nearPlaneCenter - nearWidthVector - nearHeightVector;
			Matrix<T, 3, 1> trn = nearPlaneCenter + nearWidthVector + nearHeightVector;

 			Matrix<T, 3, 1> otl = tln - origin;

			Matrix<T, 3, 1> ln = otl.Cross(tln - bln).Normalize();
			Matrix<T, 3, 1> tn = otl.Cross(trn - tln).Normalize();

			m_planes[static_cast<int>(FrustumPlane::NEAR_PLANE)]   = PlaneTempl<T>(nearPlaneCenter, forward.Reflect(forward));
			m_planes[static_cast<int>(FrustumPlane::FAR_PLANE)]    = PlaneTempl<T>(farPlaneCenter, forward);
			m_planes[static_cast<int>(FrustumPlane::LEFT_PLANE)]   = PlaneTempl<T>(origin, ln);
			m_planes[static_cast<int>(FrustumPlane::RIGHT_PLANE)]  = PlaneTempl<T>(origin, ln.Reflect(right));
			m_planes[static_cast<int>(FrustumPlane::TOP_PLANE)]    = PlaneTempl<T>(origin, tn);
			m_planes[static_cast<int>(FrustumPlane::BOTTOM_PLANE)] = PlaneTempl<T>(origin, tn.Reflect(up));

		}

		template <typename T>
		std::vector<Matrix<T, 3, 1>> FrustumTempl<T>::GetCorners(void) const
		{

			std::array<Matrix<T, 3, 1>, 8> corners;

			Matrix<T, 3, 3> A;
			Matrix<T, 3, 1> b;

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlane::LEFT_PLANE)].Normal());
			A.SetRow(1, m_planes[static_cast<int>(FrustumPlane::BOTTOM_PLANE)].Normal());
			A.SetRow(2, m_planes[static_cast<int>(FrustumPlane::NEAR_PLANE)].Normal());

			b.x() = m_planes[static_cast<int>(FrustumPlane::LEFT_PLANE)].Coefficient();
			b.y() = m_planes[static_cast<int>(FrustumPlane::BOTTOM_PLANE)].Coefficient();
			b.z() = m_planes[static_cast<int>(FrustumPlane::NEAR_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_NEAR)] = Cramer(A, b);

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlane::RIGHT_PLANE)].Normal());
			b.x() = m_planes[static_cast<int>(FrustumPlane::RIGHT_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_NEAR)] = Cramer(A, b);

			A.SetRow(1, m_planes[static_cast<int>(FrustumPlane::TOP_PLANE)].Normal());
			b.y() = m_planes[static_cast<int>(FrustumPlane::TOP_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_TOP_NEAR)] = Cramer(A, b);

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlane::LEFT_PLANE)].Normal());
			b.x() = m_planes[static_cast<int>(FrustumPlane::LEFT_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_TOP_NEAR)] = Cramer(A, b);

			A.SetRow(1, m_planes[static_cast<int>(FrustumPlane::BOTTOM_PLANE)].Normal());
			b.y() = m_planes[static_cast<int>(FrustumPlane::BOTTOM_PLANE)].Coefficient();

			A.SetRow(2, m_planes[static_cast<int>(FrustumPlane::FAR_PLANE)].Normal());
			b.z() = m_planes[static_cast<int>(FrustumPlane::FAR_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)] = Cramer(A, b);

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlane::RIGHT_PLANE)].Normal());
			b.x() = m_planes[static_cast<int>(FrustumPlane::RIGHT_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)] = Cramer(A, b);

			A.SetRow(1, m_planes[static_cast<int>(FrustumPlane::TOP_PLANE)].Normal());
			b.y() = m_planes[static_cast<int>(FrustumPlane::TOP_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)] = Cramer(A, b);

			A.SetRow(0, m_planes[static_cast<int>(FrustumPlane::LEFT_PLANE)].Normal());
			b.x() = m_planes[static_cast<int>(FrustumPlane::LEFT_PLANE)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)] = Cramer(A, b);

			return corners;

		}

		template <typename T>
		VolumeSide FrustumTempl<T>::Side(const Matrix<T, 3, 1> &x) const
		{

			PlaneSide nearSide = m_Planet[FrustumPlane::NEAR_PLANE].Side(x);

			if (nearSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide farSide = m_Planet[FrustumPlane::FAR_PLANE].Side(x);

			if (farSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide leftSide = m_Planet[FrustumPlane::LEFT_PLANE].Side(x);

			if (leftSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide rightSide = m_Planet[FrustumPlane::RIGHT_PLANE].Side(x);

			if (rightSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide topSide = m_Planet[FrustumPlane::TOP_PLANE].Side(x);

			if (topSide == PlaneSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlaneSide bottomSide = m_Planet[FrustumPlane::BOTTOM_PLANE].Side(x);

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
		const PlaneTempl<T>& FrustumTempl<T>::GetPlane(FrustumPlane Planet) const
		{
			return m_planes[static_cast<int>(Planet)];
		}

		template <typename T>
		FrustumTempl<T>* FrustumTempl<T>::Clone(void) const
		{
			return new FrustumTempl<T>(*this);
		}

		template <typename T>
		void FrustumTempl<T>::TransformAffine(Volume &volume,
											  const Matrix<T, 4, 4> &t) const
		{

			FrustumTempl<T> &f = static_cast<FrustumTempl<T>&>(volume);

			for (int fp = static_cast<int>(FrustumPlane::FIRST); fp <= static_cast<int>(FrustumPlane::LAST); fp++)
			{
				f.m_Planets[fp] = m_planes[fp].Transformt(t);
			}

		}

// 		template <typename T>
// 		const Planet<T>& Frustumt<T>::Near(void) const
// 		{
// 			return m_near;
// 		}
// 
// 		template <typename T>
// 		const Planet<T>& Frustumt<T>::Far(void) const
// 		{
// 			return m_far;
// 		}
// 
// 		template <typename T>
// 		const Planet<T>& Frustumt<T>::Left(void) const
// 		{
// 			return m_left;
// 		}
// 
// 		template <typename T>
// 		const Planet<T>& Frustumt<T>::Right(void) const
// 		{
// 			return m_right;
// 		}
// 
// 		template <typename T>
// 		const Planet<T>& Frustumt<T>::Top(void) const
// 		{
// 			return m_top;
// 		}
// 
// 		template <typename T>
// 		const Planet<T>& Frustumt<T>::Bottom(void) const
// 		{
// 			return m_bottom;
// 		}

	}

}