#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		class QuaternionTempl;

		typedef QuaternionTempl<float> Quaternionf;
		typedef QuaternionTempl<double> Quaterniond;

		typedef QuaternionTempl<Real> Quaternion;

		template <typename T = Real>
		class TransformTempl;

		typedef TransformTempl<float> Transformtf;
		typedef TransformTempl<double> Transformtd;
		typedef TransformTempl<Real> Transform;

		template <typename T = Real>
		class AABBTempl;

		typedef AABBTempl<float> AABBf;
		typedef AABBTempl<double> AABBd;
		typedef AABBTempl<Real> AABB;

		template <typename T = Real>
		class PlaneTempl;

		typedef PlaneTempl<float> Planetf;
		typedef PlaneTempl<double> Planetd;
		typedef PlaneTempl<Real> Plane;

		template <typename T = Real>
		class FrustumTempl;

		typedef FrustumTempl<float> Frustumf;
		typedef FrustumTempl<double> Frustumd;
		typedef FrustumTempl<Real> Frustum;

		template <typename T = Real>
		class RayTempl;

		typedef RayTempl<float> Rayf;
		typedef RayTempl<double> Rayd;
		typedef RayTempl<Real> Ray;

	}

}