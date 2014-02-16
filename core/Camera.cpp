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

void Camera::LookAt(const mye::math::Vector3f &position,
			const mye::math::Vector3f &up,
			const mye::math::Vector3f &target)
{

	Vector3f z = (target - position).Normalized();
	Vector3f x = up.Cross(z);
	Vector3f y = z.Cross(x);

	Matrix3f cam;

	cam.SetColumn(0, x);
	cam.SetColumn(1, y);
	cam.SetColumn(2, z);

	m_orientation = Quaternionf(cam);

	m_position = position;

	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;

}

mye::math::Quaternionf Camera::GetOrientation(void) const
{
	return m_orientation;
}

void Camera::SetOrientation(const mye::math::Quaternionf &direction)
{
	m_orientation        = direction;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;
}

mye::math::Vector3f Camera::GetPosition(void) const
{
	return m_position;
}

void Camera::SetPosition(const mye::math::Vector3f &position)
{
	m_position           = position;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;
}

void Camera::Pitch(float angle)
{

// 	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
// 	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternionf pitchQuaternion(Right(), angle);
		
		/*(cosHalfAngle,
		sinHalfAngle,
		0,
		0);*/

	m_orientation        = pitchQuaternion * m_orientation;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;

}

void Camera::Yaw(float angle)
{

// 	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
// 	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternionf yawQuaternion(Up(), angle);
		
/*
		(cosHalfAngle,
		0,
		sinHalfAngle,
		0);*/

	m_orientation        = yawQuaternion * m_orientation;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;

}

void Camera::Roll(float angle)
{

// 	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
// 	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternionf rollQuaternion(Forward(), angle);
		
		
	/*(cosHalfAngle,
		0,
		0,
		sinHalfAngle);*/

	m_orientation        = rollQuaternion * m_orientation;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;

}

/* Projection */

void Camera::SetProjection(float fovy, float aspect, float zNear, float zFar)
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

void Camera::SetNearClipDistance(float near)
{
	m_nearClipDistance         = near;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

float Camera::GetFarClipDistance(void) const
{
	return m_farClipDistance;
}

void Camera::SetFarClipDistance(float far)
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

float Camera::GetClipAspectRatio(void) const
{
	return m_aspectRatio;
}

void Camera::SetClipAspectRatio(float aspect)
{
	m_aspectRatio              = aspect;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

float Camera::GetFovY(void) const
{
	return Degrees(m_fovY);
}

float Camera::GetFovX(void) const
{
	return Degrees(2.0f * Arctangent(m_aspectRatio * Tangent(0.5f * m_fovY)));
}

float  Camera::GetFovYRadians(void) const
{
	return m_fovY;
}

float  Camera::GetFovXRadians(void) const
{
	return 2.0f * Arctangent(m_aspectRatio * Tangent(0.5f * m_fovY));
}