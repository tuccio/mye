#pragma once

#include "AlignedAllocator.h"

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		class Camera
		{

		public:

			__MYE_DECLARE_ALIGNED_16

			Camera(void);
			~Camera(void);

			void LookAt(const mye::math::Vector3 & position,
				const mye::math::Vector3 & up,
				const mye::math::Vector3 & target);

			mye::math::Quaternion GetOrientation(void) const;
			void SetOrientation(const mye::math::Quaternion &orientation);

			mye::math::Quaternion & Orientation(void);

			mye::math::Vector3 GetPosition(void) const;
			void SetPosition(const mye::math::Vector3 & position);

			mye::math::Vector3 & Position(void);

			void SetProjection(mye::math::Real fovy, mye::math::Real aspect, mye::math::Real zNear, mye::math::Real zFar);

			mye::math::Real GetNearClipDistance(void) const;
			void SetNearClipDistance(mye::math::Real near);

			mye::math::Real GetFarClipDistance(void) const;
			void SetFarClipDistance(mye::math::Real far);

			void SetClipDistances(mye::math::Real near, mye::math::Real far);

			mye::math::Real GetClipAspectRatio(void) const;
			void SetClipAspectRatio(mye::math::Real aspect);

			mye::math::Real GetFovY(void) const;
			void SetFovY(mye::math::Real fovy);

			mye::math::Real GetFovX(void) const;

			mye::math::Real GetFovYRadians(void) const;
			mye::math::Real GetFovXRadians(void) const;

			void Roll(mye::math::Real angle);
			void Pitch(mye::math::Real angle);
			void Yaw(mye::math::Real angle);

			inline mye::math::Vector3 Up(void) const;
			inline mye::math::Vector3 Right(void) const;
			inline mye::math::Vector3 Forward(void) const;

			inline void UpdateView(void);
			inline void UpdateProjection(void);
			inline void UpdateFrustum(void);

			inline mye::math::Matrix4 GetViewMatrix(void) const;
			inline mye::math::Matrix4 GetProjectionMatrix(void) const;

			inline const mye::math::Matrix4 & GetViewMatrix(void);
			inline const mye::math::Matrix4 & GetProjectionMatrix(void);

			inline mye::math::Frustum         GetFrustum(void) const;
			inline const mye::math::Frustum & GetFrustum(void);

			mye::math::Ray RayCast(const mye::math::Vector2 &screenCoords) const;

		private:

			mye::math::Quaternion m_orientation;
			mye::math::Vector3    m_position;

			mye::math::Real m_fovY;
			mye::math::Real m_aspectRatio;
			mye::math::Real m_nearClipDistance;
			mye::math::Real m_farClipDistance;

			mye::math::Matrix4 m_viewMatrix;
			mye::math::Matrix4 m_projectionMatrix;

			bool m_viewMatrixUptodate;
			bool m_projectionMatrixUptodate;
			bool m_frustumUptodate;

			mye::math::Frustum m_frustum;

		};

	}

}

#include "Camera.inl"
