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

		typedef QuaternionTempl<float>  __MYE_MATH_SSE_ALIGNED(16) Quaternionf;
		typedef QuaternionTempl<double> __MYE_MATH_SSE_ALIGNED(16) Quaterniond;

		typedef QuaternionTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Quaternion;

		template <typename T = Real>
		class TransformTempl;

		typedef TransformTempl<float>  Transformtf;
		typedef TransformTempl<double> Transformtd;

		typedef TransformTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Transform;

		template <typename T = Real>
		class AABBTempl;

		typedef AABBTempl<float>  __MYE_MATH_SSE_ALIGNED(16) AABBf;
		typedef AABBTempl<double> __MYE_MATH_SSE_ALIGNED(16) AABBd;

		typedef AABBTempl<Real> __MYE_MATH_SSE_ALIGNED(16) AABB;

		template <typename T = Real>
		class PlaneTempl;

		typedef PlaneTempl<float>  __MYE_MATH_SSE_ALIGNED(16) Planef;
		typedef PlaneTempl<double> __MYE_MATH_SSE_ALIGNED(16) Planed;

		typedef PlaneTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Plane;

		template <typename T = Real>
		class FrustumTempl;

		typedef FrustumTempl<float>  __MYE_MATH_SSE_ALIGNED(16) Frustumf;
		typedef FrustumTempl<double> __MYE_MATH_SSE_ALIGNED(16) Frustumd;
		typedef FrustumTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Frustum;

		template <typename T = Real>
		class RayTempl;

		typedef RayTempl<float>  __MYE_MATH_SSE_ALIGNED(16) Rayf;
		typedef RayTempl<double> __MYE_MATH_SSE_ALIGNED(16) Rayd;

		typedef RayTempl<Real> __MYE_MATH_SSE_ALIGNED(16) Ray;

	}

}