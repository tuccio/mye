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

	SetFrustum(90.0f, 1.0f, 1.0f, 100.0f);

	m_viewMatrixUptodate       = true;
	m_projectionMatrixUptodate = true;

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

}

mye::math::Quaternionf Camera::GetOrientation(void) const
{
	return m_orientation;
}

void Camera::SetOrientation(const mye::math::Quaternionf &direction)
{
	m_orientation = direction;
}

mye::math::Vector3f Camera::GetPosition(void) const
{
	return m_position;
}

void Camera::SetPosition(const mye::math::Vector3f &position)
{
	m_position           = position;
	m_viewMatrixUptodate = false;
}

void Camera::Pitch(float angle)
{

	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternionf pitchQuaternion(cosHalfAngle,
		sinHalfAngle,
		0,
		0);

	m_orientation        = pitchQuaternion * m_orientation;
	m_viewMatrixUptodate = false;

}

void Camera::Yaw(float angle)
{

	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternionf yawQuaternion(cosHalfAngle,
		0,
		sinHalfAngle,
		0);

	m_orientation        = yawQuaternion * m_orientation;
	m_viewMatrixUptodate = false;

}

void Camera::Roll(float angle)
{

	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Quaternionf rollQuaternion(cosHalfAngle,
		0,
		0,
		sinHalfAngle);

	m_orientation        = rollQuaternion * m_orientation;
	m_viewMatrixUptodate = false;

}

void Camera::UpdateView(void)
{
	
	m_viewMatrix = RotationMatrix4(m_orientation);

	m_viewMatrix(3, 0) = - Vector3f(m_viewMatrix(0, 0),
		m_viewMatrix(0, 1),
		m_viewMatrix(0, 2)).Dot(m_position);

	m_viewMatrix(3, 1) = - Vector3f(m_viewMatrix(1, 0),
		m_viewMatrix(1, 1),
		m_viewMatrix(1, 2)).Dot(m_position);

	m_viewMatrix(3, 2) = - Vector3f(m_viewMatrix(2, 0),
		m_viewMatrix(2, 1),
		m_viewMatrix(2, 2)).Dot(m_position);

	m_viewMatrixUptodate = true;

}

/* Projection */

void Camera::SetFrustum(float fovy, float aspect, float zNear, float zFar)
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
	m_nearClipDistance = near;
	m_projectionMatrixUptodate = false;
}

float Camera::GetFarClipDistance(void) const
{
	return m_farClipDistance;
}

void Camera::SetFarClipDistance(float far)
{
	m_farClipDistance = far;
	m_projectionMatrixUptodate = false;
}

void Camera::SetClipDistances(float near, float far)
{
	m_nearClipDistance = near;
	m_farClipDistance  = far;
	m_projectionMatrixUptodate = false;
}

float Camera::GetClipAspectRatio(void) const
{
	return m_aspectRatio;
}

void Camera::SetClipAspectRatio(float aspect)
{
	m_aspectRatio = aspect;
	m_projectionMatrixUptodate = false;
}

float Camera::GetFovY(void) const
{
	return Degrees(m_fovY);
}

float Camera::GetFovX(void) const
{
	return Degrees(2.0f * Arctangent(m_aspectRatio * Tangent(0.5f * m_fovY)));
}

void Camera::UpdateProjection(void)
{

	float yScale = 1.0f / Tangent(m_fovY * 0.5f);
	float xScale = yScale / m_aspectRatio;
	float invDepth = 1.0f / (m_farClipDistance - m_nearClipDistance);

	m_projectionMatrix(0, 0) = xScale;
	m_projectionMatrix(1, 1) = yScale;
	m_projectionMatrix(2, 2) = m_farClipDistance * invDepth;
	m_projectionMatrix(3, 2) = - m_nearClipDistance * m_farClipDistance * invDepth;
	m_projectionMatrix(2, 3) = 1;

	m_projectionMatrixUptodate = true;

}