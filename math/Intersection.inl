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

	}

}