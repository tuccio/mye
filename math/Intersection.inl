namespace mye
{

	namespace math
	{

		template <typename T>
		bool Intersect(const RayTempl<T> &ray,
			const Sphere<T> &sphere)
		{
			return Intersect(Rayt, sphere, Matrix<T, 3, 1>());
		}

		template <typename T>
		bool Intersect(const RayTempl<T> &ray,
			const Sphere<T> &sphere,
			Matrix<T, 3, 1> &intersectionPoint)
		{

			Vector3f os = ray.GetOrigin() - sphere.GetCenter();

			float a = ray.GetDirection().Dot(ray.GetDirection());
			float b = 2.0f * (ray.GetDirection() * ray.GetOrigin());
			float c = ray.GetOrigin().Dot(ray.GetOrigin())

		}

		template <typename T>
		VolumeSide Intersect(const AABBTempl<T> &a,
			const AABBTempl<T> &b)
		{
			throw; // TODO
		}

		template <typename T>
		VolumeSide Intersect(const AABBTempl<T> &aabb,
			const FrustumTempl<T> &frustum)
		{

			VolumeSide r = VolumeSide::INSIDE;

			for (int i = 0; i < 6; i++)
			{

				const PlaneTempl<T> &plane = frustum.GetPlane(static_cast<FrustumPlane>(i));
				Matrix<T, 3, 1> normal = plane.Normal();
				T d = plane.Coefficient();

				Matrix<T, 3, 1> min = aabb.GetMinimum();
				Matrix<T, 3, 1> max = aabb.GetMaximum();

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
		VolumeSide Intersect(const FrustumTempl<T> &frustum,
			const AABBTempl<T> &aabb)
		{
			return Intersect(aabb, frustum);
		}

		template <typename T>
		VolumeSide FrustumTempl<T>::Intersects(const AABBTempl<T> &aabb) const
		{
			return Intersect(aabb, *this);
		}

		template <typename T>
		VolumeSide AABBTempl<T>::Intersects(const FrustumTempl<T> &frustum) const
		{
			return Intersect(*this, frustum);
		}

		template <typename T>
		VolumeSide Intersect(const FrustumTempl<T> &a,
			const FrustumTempl<T> &b)
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

					return mye::math::Intersect<T>(static_cast<const AABBTempl<T>&>(*this),
						static_cast<const AABBTempl<T>&>(volume));
					break;

				case VolumeType::FRUSTUM:

					return mye::math::Intersect<T>(static_cast<const AABBTempl<T>&>(*this),
						static_cast<const FrustumTempl<T>&>(volume));

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

					return mye::math::Intersect<T>(static_cast<const FrustumTempl<T>&>(*this),
						static_cast<const AABBTempl<T>&>(volume));
					break;

				case VolumeType::FRUSTUM:

					return mye::math::Intersect<T>(static_cast<const FrustumTempl<T>&>(*this),
						static_cast<const FrustumTempl<T>&>(volume));
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