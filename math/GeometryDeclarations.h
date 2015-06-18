#pragma once

namespace mye
{

	namespace math
	{

		enum class VolumeSide
		{
			INSIDE,
			INTERSECT,
			OUTSIDE
		};

		template <typename T>
		class QuaternionTempl;

		typedef QuaternionTempl<float>  Quaternionf;
		typedef QuaternionTempl<double> Quaterniond;

		typedef QuaternionTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Quaternion;

		template <typename T = Real>
		class TransformTempl;

		typedef TransformTempl<float>  Transformtf;
		typedef TransformTempl<double> Transformtd;

		typedef TransformTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Transform;

		template <typename T = Real>
		class AABBTempl;

		typedef AABBTempl<float>  AABBf;
		typedef AABBTempl<double> AABBd;

		typedef AABBTempl<Real> __MYE_MATH_SSE_ALIGNED(16) AABB;

		template <typename T = Real>
		class PlaneTempl;

		typedef PlaneTempl<float>  Planetf;
		typedef PlaneTempl<double> Planetd;

		typedef PlaneTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Plane;

		template <typename T = Real>
		class FrustumTempl;

		typedef FrustumTempl<float>  Frustumf;
		typedef FrustumTempl<double> Frustumd;
		typedef FrustumTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Frustum;

		template <typename T = Real>
		class RayTempl;

		typedef RayTempl<float>  Rayf;
		typedef RayTempl<double> Rayd;

		typedef RayTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Ray;

	}

}