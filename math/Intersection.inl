namespace mye
{

	namespace math
	{

		template <typename T>
		bool Intersect(const Ray<T> &ray,
			const Sphere<T> &sphere)
		{
			return Intersect(ray, sphere, Matrix<T, 3, 1>());
		}

		template <typename T>
		bool Intersect(const Ray<T> &ray,
			const Sphere<T> &sphere,
			Matrix<T, 3, 1> &intersectionPoint)
		{

			Vector3f os = ray.GetOrigin() - sphere.GetCenter();

			float a = ray.GetDirection().Dot(ray.GetDirection());
			float b = 2.0f * (ray.GetDirection() * ray.GetOrigin());
			float c = ray.GetOrigin().Dot(ray.GetOrigin())

		}

		template <typename T>
		VolumeSide Intersect(const AABB<T> &aabb,
			const Frustum<T> &frustum)
		{

			VolumeSide r = VolumeSide::INSIDE;

			for (int i = 0; i < 6; i++)
			{

				const Plane<T> &plane = frustum.GetPlane(static_cast<FrustumPlanes>(i));
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

	}

}