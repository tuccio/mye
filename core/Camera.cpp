#include "Camera.h"

#include <cassert>
#include <cmath>

using namespace mye::core;
using namespace mye::math;

Camera::Camera(void) :
	m_orientation(1.0f, 0.0f, 0.0f, 0.0f),
	m_position(0.0f, 0.0f, 0.0f),
	m_viewMatrix(1.0f),
	m_projectionMatrix(0.0f)
{

	SetProjection(90.0f, 1.0f, 1.0f, 100.0f);

	m_viewMatrixUptodate       = false;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;

}


Camera::~Camera(void)
{
}

/* View */

void Camera::LookAt(const mye::math::Vector3 & position,
					const mye::math::Vector3 & up,
					const mye::math::Vector3 & target)
{

	Vector3 z = (target - position).Normalize();
	Vector3 x = up.Cross(z);
	Vector3 y = z.Cross(x);

	Matrix3 cam;

	cam.SetColumn(0, x);
	cam.SetColumn(1, y);
	cam.SetColumn(2, z);

	m_orientation = Quaternion(cam);

	m_position = position;

	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;

}

Quaternion Camera::GetOrientation(void) const
{
	return m_orientation;
}

void Camera::SetOrientation(const mye::math::Quaternion & direction)
{
	m_orientation        = direction;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;
}

Quaternion & Camera::Orientation(void)
{
	return m_orientation;
}

Vector3 Camera::GetPosition(void) const
{
	return m_position;
}

void Camera::SetPosition(const mye::math::Vector3 & position)
{
	m_position           = position;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;
}

Vector3 & Camera::Position(void)
{
	return m_position;
}

void Camera::Pitch(mye::math::Real angle)
{

// 	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
// 	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternion pitchQuaternion(Right(), angle);
		
		/*(cosHalfAngle,
		sinHalfAngle,
		0,
		0);*/

	m_orientation        = pitchQuaternion * m_orientation;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;

}

void Camera::Yaw(mye::math::Real angle)
{

// 	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
// 	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternion yawQuaternion(Up(), angle);
		
/*
		(cosHalfAngle,
		0,
		sinHalfAngle,
		0);*/

	m_orientation        = yawQuaternion * m_orientation;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;

}

void Camera::Roll(mye::math::Real angle)
{

// 	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
// 	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternion rollQuaternion(Forward(), angle);
		
		
	/*(cosHalfAngle,
		0,
		0,
		sinHalfAngle);*/

	m_orientation        = rollQuaternion * m_orientation;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;

}

/* Projection */

void Camera::SetProjection(mye::math::Real fovy, mye::math::Real aspect, mye::math::Real zNear, mye::math::Real zFar)
{
	m_fovY             = Radians(fovy);
	m_aspectRatio      = aspect;
	m_nearClipDistance = zNear;
	m_farClipDistance  = zFar;
}

float Camera::GetNearClipDistance(void) const
{
	return m_nearClipDistance;
}

void Camera::SetNearClipDistance(mye::math::Real near)
{
	m_nearClipDistance         = near;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

mye::math::Real Camera::GetFarClipDistance(void) const
{
	return m_farClipDistance;
}

void Camera::SetFarClipDistance(mye::math::Real far)
{
	m_farClipDistance          = far;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

void Camera::SetClipDistances(float near, float far)
{
	m_nearClipDistance         = near;
	m_farClipDistance          = far;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

mye::math::Real Camera::GetClipAspectRatio(void) const
{
	return m_aspectRatio;
}

void Camera::SetClipAspectRatio(mye::math::Real aspect)
{
	m_aspectRatio              = aspect;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

mye::math::Real Camera::GetFovY(void) const
{
	return Degrees(m_fovY);
}

void Camera::SetFovY(mye::math::Real fovy)
{
	m_fovY                     = Radians(fovy);
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

mye::math::Real Camera::GetFovX(void) const
{
	return Degrees(2.0f * Arctangent(m_aspectRatio * Tangent(0.5f * m_fovY)));
}

mye::math::Real Camera::GetFovYRadians(void) const
{
	return m_fovY;
}

mye::math::Real Camera::GetFovXRadians(void) const
{
	return 2.0f * Arctangent(m_aspectRatio * Tangent(0.5f * m_fovY));
}

Ray Camera::RayCast(const mye::math::Vector2 & screenCoords) const
{

	Matrix4 invViewProjMatrix = (GetProjectionMatrix() * GetViewMatrix()).Inverse();

	Vector2 deviceCoords(2 * screenCoords.x() - 1,
						 1 - 2 * screenCoords.y());

	Vector4 nearH = invViewProjMatrix * Vector4(deviceCoords.xy(), 0, 1);
	Vector4 farH  = invViewProjMatrix * Vector4(deviceCoords.xy(), 1, 1);

	Vector3 near = nearH.xyz() / nearH.w();
	Vector3 far  =  farH.xyz() /  farH.w();

	return Ray(m_position, (far - near).Normalize());

}