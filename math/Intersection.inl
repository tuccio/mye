namespace mye
{

	namespace math
	{

		template <typename T>
		bool Intersect(const Rayt<T> &Rayt,
			const Sphere<T> &sphere)
		{
			return Intersect(Rayt, sphere, Matrix<T, 3, 1>());
		}

		template <typename T>
		bool Intersect(const Rayt<T> &Rayt,
			const Sphere<T> &sphere,
			Matrix<T, 3, 1> &intersectionPoint)
		{

			Vector3f os = Rayt.GetOrigin() - sphere.GetCenter();

			float a = Rayt.GetDirection().Dot(Rayt.GetDirection());
			float b = 2.0f * (Rayt.GetDirection() * Rayt.GetOrigin());
			float c = Rayt.GetOrigin().Dot(Rayt.GetOrigin())

		}

		template <typename T>
		VolumeSide Intersect(const AABBt<T> &a,
			const AABBt<T> &b)
		{
			throw; // TODO
		}

		template <typename T>
		VolumeSide Intersect(const AABBt<T> &AABBt,
			const Frustumt<T> &frustum)
		{

			VolumeSide r = VolumeSide::INSIDE;

			for (int i = 0; i < 6; i++)
			{

				const Planet<T> &Planet = frustum.GetPlanet(static_cast<FrustumPlanets>(i));
				Matrix<T, 3, 1> normal = Planet.Normal();
				T d = Planet.Coefficient();

				Matrix<T, 3, 1> min = AABBt.GetMinimum();
				Matrix<T, 3, 1> max = AABBt.GetMaximum();

				Matrix<T, 3, 1> n, p;

				if (normal.x() >= 0)
				{
					n.x() = min.x();
					p.x() = max.x();
				}
				else
				{
					n.x() = max.x();
					p.x() = min.x();
				}

				if (normal.y() >= 0)
				{
					n.y() = min.y();
					p.y() = max.y();
				}
				else
				{
					n.y() = max.y();
					p.y() = min.y();
				}

				if (normal.z() >= 0)
				{
					n.z() = min.z();
					p.z() = max.z();
				}
				else
				{
					n.z() = max.z();
					p.z() = min.z();
				}

				if (normal.Dot(n) + d > 0)
				{
					return VolumeSide::OUTSIDE;
				}

				if (normal.Dot(p) + d >= 0)
				{
					r = VolumeSide::INTERSECT;
				}

			}

			return r;

		}

		template <typename T>
		VolumeSide Intersect(const Frustumt<T> &frustum,
			const AABBt<T> &AABBt)
		{
			return Intersect(AABBt, frustum);
		}

		template <typename T>
		VolumeSide Frustumt<T>::Intersects(const AABBt<T> &AABBt) const
		{
			return Intersect(AABBt, *this);
		}

		template <typename T>
		VolumeSide AABBt<T>::Intersects(const Frustumt<T> &frustum) const
		{
			return Intersect(*this, frustum);
		}

		template <typename T>
		VolumeSide Intersect(const Frustumt<T> &a,
			const Frustumt<T> &b)
		{
			throw; // TODO
		}

		template <typename T>
		VolumeSide Volume<T>::Intersect(const Volume<T> &volume) const
		{

			if (m_volumeType == VolumeType::AABBt)
			{

				switch(volume.m_volumeType)
				{

				case VolumeType::AABBt:

					return mye::math::Intersect<T>(static_cast<const AABBt<T>&>(*this),
						static_cast<const AABBt<T>&>(volume));
					break;

				case VolumeType::FRUSTUM:

					return mye::math::Intersect<T>(static_cast<const AABBt<T>&>(*this),
						static_cast<const Frustumt<T>&>(volume));

					break;

				default:
					break;

				}

			}
			else if (m_volumeType == VolumeType::FRUSTUM)
			{

				switch(volume.m_volumeType)
				{

				case VolumeType::AABBt:

					return mye::math::Intersect<T>(static_cast<const Frustumt<T>&>(*this),
						static_cast<const AABBt<T>&>(volume));
					break;

				case VolumeType::FRUSTUM:

					return mye::math::Intersect<T>(static_cast<const Frustumt<T>&>(*this),
						static_cast<const Frustumt<T>&>(volume));
					break;

				default:
					break;

				}

			}

			assert(false && "Unknown volume type");

			return VolumeSide::OUTSIDE;

		}

	}

}