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
			const Matrix<T, 3, 1>& forward,
			const Matrix<T, 3, 1>& up,
			const Matrix<T, 3, 1>& right,
			T nearPlanetDistance,
			T farPlanetDistance,
			T fovX,
			T fovY) :
		Volume(VolumeType::FRUSTUM)
		{

			/* Construct Planets */

			T tgFovX = Abs(Tangent(Radians(fovX * 0.5f)));
			T tgFovY = Abs(Tangent(Radians(fovY * 0.5f)));

			T nearHeight = tgFovY * nearPlanetDistance;
			T nearWidth  = tgFovX * nearPlanetDistance;

			Matrix<T, 3, 1> nearHeightVector = nearHeight * up;
			Matrix<T, 3, 1> nearWidthVector = nearWidth * right;

			Matrix<T, 3, 1> nearPlanetCenter = origin + nearPlanetDistance * forward;
			Matrix<T, 3, 1> farPlanetCenter  = origin + farPlanetDistance * forward;

			Matrix<T, 3, 1> tln = nearPlanetCenter - nearWidthVector + nearHeightVector;
			Matrix<T, 3, 1> bln = nearPlanetCenter - nearWidthVector - nearHeightVector;
			Matrix<T, 3, 1> trn = nearPlanetCenter + nearWidthVector + nearHeightVector;

 			Matrix<T, 3, 1> otl = tln - origin;

			Matrix<T, 3, 1> ln = otl.Cross(tln - bln).Normalize();
			Matrix<T, 3, 1> tn = otl.Cross(trn - tln).Normalize();

			m_Planets[static_cast<int>(FrustumPlane::NEAR_Planet)]   = PlaneTempl<T>(nearPlanetCenter, forward.Reflect(forward));
			m_Planets[static_cast<int>(FrustumPlane::FAR_Planet)]    = PlaneTempl<T>(farPlanetCenter, forward);
			m_Planets[static_cast<int>(FrustumPlane::LEFT_Planet)]   = PlaneTempl<T>(origin, ln);
			m_Planets[static_cast<int>(FrustumPlane::RIGHT_Planet)]  = PlaneTempl<T>(origin, ln.Reflect(right));
			m_Planets[static_cast<int>(FrustumPlane::TOP_Planet)]    = PlaneTempl<T>(origin, tn);
			m_Planets[static_cast<int>(FrustumPlane::BOTTOM_Planet)] = PlaneTempl<T>(origin, tn.Reflect(up));

		}

		template <typename T>
		std::vector<Matrix<T, 3, 1>> FrustumTempl<T>::GetCorners(void) const
		{

			std::vector<Matrix<T, 3, 1>> corners(8);

			Matrix<T, 3, 3> A;
			Matrix<T, 3, 1> b;

			A.SetRow(0, m_Planets[static_cast<int>(FrustumPlane::LEFT_Planet)].Normal());
			A.SetRow(1, m_Planets[static_cast<int>(FrustumPlane::BOTTOM_Planet)].Normal());
			A.SetRow(2, m_Planets[static_cast<int>(FrustumPlane::NEAR_Planet)].Normal());

			b.x() = m_Planets[static_cast<int>(FrustumPlane::LEFT_Planet)].Coefficient();
			b.y() = m_Planets[static_cast<int>(FrustumPlane::BOTTOM_Planet)].Coefficient();
			b.z() = m_Planets[static_cast<int>(FrustumPlane::NEAR_Planet)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_NEAR)] = Cramer(A, b);

			A.SetRow(0, m_Planets[static_cast<int>(FrustumPlane::RIGHT_Planet)].Normal());
			b.x() = m_Planets[static_cast<int>(FrustumPlane::RIGHT_Planet)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_NEAR)] = Cramer(A, b);

			A.SetRow(1, m_Planets[static_cast<int>(FrustumPlane::TOP_Planet)].Normal());
			b.y() = m_Planets[static_cast<int>(FrustumPlane::TOP_Planet)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_TOP_NEAR)] = Cramer(A, b);

			A.SetRow(0, m_Planets[static_cast<int>(FrustumPlane::LEFT_Planet)].Normal());
			b.x() = m_Planets[static_cast<int>(FrustumPlane::LEFT_Planet)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_TOP_NEAR)] = Cramer(A, b);

			A.SetRow(1, m_Planets[static_cast<int>(FrustumPlane::BOTTOM_Planet)].Normal());
			b.y() = m_Planets[static_cast<int>(FrustumPlane::BOTTOM_Planet)].Coefficient();

			A.SetRow(2, m_Planets[static_cast<int>(FrustumPlane::FAR_Planet)].Normal());
			b.z() = m_Planets[static_cast<int>(FrustumPlane::FAR_Planet)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_BOTTOM_FAR)] = Cramer(A, b);

			A.SetRow(0, m_Planets[static_cast<int>(FrustumPlane::RIGHT_Planet)].Normal());
			b.x() = m_Planets[static_cast<int>(FrustumPlane::RIGHT_Planet)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_BOTTOM_FAR)] = Cramer(A, b);

			A.SetRow(1, m_Planets[static_cast<int>(FrustumPlane::TOP_Planet)].Normal());
			b.y() = m_Planets[static_cast<int>(FrustumPlane::TOP_Planet)].Coefficient();

			corners[static_cast<int>(FrustumCorners::RIGHT_TOP_FAR)] = Cramer(A, b);

			A.SetRow(0, m_Planets[static_cast<int>(FrustumPlane::LEFT_Planet)].Normal());
			b.x() = m_Planets[static_cast<int>(FrustumPlane::LEFT_Planet)].Coefficient();

			corners[static_cast<int>(FrustumCorners::LEFT_TOP_FAR)] = Cramer(A, b);

			return corners;

		}

		template <typename T>
		VolumeSide FrustumTempl<T>::Side(const Matrix<T, 3, 1> &x) const
		{

			PlanetSide nearSide = m_Planet[FrustumPlane::NEAR_Planet].Side(x);

			if (nearSide == PlanetSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlanetSide farSide = m_Planet[FrustumPlane::FAR_Planet].Side(x);

			if (farSide == PlanetSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlanetSide leftSide = m_Planet[FrustumPlane::LEFT_Planet].Side(x);

			if (leftSide == PlanetSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlanetSide rightSide = m_Planet[FrustumPlane::RIGHT_Planet].Side(x);

			if (rightSide == PlanetSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlanetSide topSide = m_Planet[FrustumPlane::TOP_Planet].Side(x);

			if (topSide == PlanetSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			PlanetSide bottomSide = m_Planet[FrustumPlane::BOTTOM_Planet].Side(x);

			if (bottomSide == PlanetSide::FRONT)
			{
				return VolumeSide::OUTSIDE;
			}

			if (topSide    == PlanetSide::INSIDE ||
				bottomSide == PlanetSide::INSIDE ||
				rightSide  == PlanetSide::INSIDE ||
				leftSide   == PlanetSide::INSIDE ||
				nearSide   == PlanetSide::INSIDE ||
				farSide    == PlanetSide::INSIDE)
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
			return m_Planets[static_cast<int>(Planet)];
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
				f.m_Planets[fp] = m_Planets[fp].Transformt(t);
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