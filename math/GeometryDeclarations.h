#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class Quaterniont;

		typedef Quaterniont<float> Quaternionf;
		typedef Quaterniont<double> Quaterniond;

		typedef Quaterniont<Real> Quaternion;

		template <typename T = Real>
		class Transformt;

		typedef Transformt<float> Transformtf;
		typedef Transformt<double> Transformtd;
		typedef Transformt<Real> Transform;

		template <typename T = Real>
		class AABBt;

		typedef AABBt<float> AABBf;
		typedef AABBt<double> AABBd;
		typedef AABBt<Real> AABB;

		template <typename T = Real>
		class Planet;

		typedef Planet<float> Planetf;
		typedef Planet<double> Planetd;
		typedef Planet<Real> Plane;

		template <typename T = Real>
		class Frustumt;

		typedef Frustumt<float> Frustumf;
		typedef Frustumt<double> Frustumd;
		typedef Frustumt<Real> Frustum;

		template <typename T = Real>
		class Rayt;

		typedef Rayt<float> Rayf;
		typedef Rayt<double> Rayd;
		typedef Rayt<Real> Ray;

	}

}