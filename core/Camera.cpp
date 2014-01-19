#include "Camera.h"

#include <cassert>
#include <Eigen/Geometry>

using namespace mye::core;

Camera::Camera(void) :
	m_orientation(1.0f, 0.0f, 0.0f, 0.0f),
	m_position(0.0f, 0.0f, 0.0f)
{

	m_nearClipDistance = 1.0f;
	m_farClipDistance = 100.0f;

	m_viewMatrix = Eigen::Matrix4f::Identity();
	m_projectionMatrix = Eigen::Matrix4f::Identity();

	SetFrustum(90.0f, 1.0f, 1.0f, 100.0f);

}


Camera::~Camera(void)
{
}

/* View */

void Camera::LookAt(const Eigen::Vector3f &position,
			const Eigen::Vector3f &up,
			const Eigen::Vector3f &target)
{

	Eigen::Matrix3f cam;
	cam.row(2) = (target - position).normalized();
	cam.row(0) = up.cross(cam.row(2));
	cam.row(1) = cam.row(2).cross(cam.row(0));

	m_orientation = cam;
	m_position = position;

}

Eigen::Quaternionf Camera::GetOrientation(void) const
{
	return m_orientation;
}

void Camera::SetOrientation(const Eigen::Quaternionf &direction)
{
	m_orientation = direction;
}

Eigen::Vector3f Camera::GetPosition(void) const
{
	return m_position;
}

void Camera::SetPosition(const Eigen::Vector3f &position)
{
	m_position = position;
}

void Camera::Pitch(float angle)
{

	float cosHalfAngle = cos(angle * 0.5f);
	float sinHalfAngle = sin(angle * 0.5f);

	Eigen::Quaternionf pitchQuaternion(cosHalfAngle,
		sinHalfAngle,
		0,
		0);

	m_orientation = m_orientation * pitchQuaternion;

}

void Camera::Yaw(float angle)
{

	float cosHalfAngle = cos(angle * 0.5f);
	float sinHalfAngle = sin(angle * 0.5f);

	Eigen::Quaternionf yawQuaternion(cosHalfAngle,
		0,
		sinHalfAngle,
		0);

	m_orientation = m_orientation * yawQuaternion;

}

void Camera::Roll(float angle)
{

	float cosHalfAngle = cos(angle * 0.5f);
	float sinHalfAngle = sin(angle * 0.5f);

	Eigen::Quaternionf rollQuaternion(cosHalfAngle,
		0,
		0,
		sinHalfAngle);

	m_orientation = m_orientation * rollQuaternion;

}

void Camera::UpdateView(void)
{
	m_viewMatrix.block<3, 3>(0, 0) = m_orientation.toRotationMatrix();
	m_viewMatrix(0, 3) = - m_position.dot(m_viewMatrix.block<3, 1>(0, 0));
	m_viewMatrix(1, 3) = - m_position.dot(m_viewMatrix.block<3, 1>(0, 1));
	m_viewMatrix(2, 3) = - m_position.dot(m_viewMatrix.block<3, 1>(0, 2));
}

/* Projection */

void Camera::SetFrustum(float fovy, float aspect, float zNear, float zFar)
{
	m_fovY             = fovy;
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
}

float Camera::GetFarClipDistance(void) const
{
	return m_farClipDistance;
}

void Camera::SetFarClipDistance(float far)
{
	m_farClipDistance = far;
}

void Camera::SetClipDistances(float near, float far)
{
	m_nearClipDistance = near;
	m_farClipDistance  = far;
}

float Camera::GetClipAspectRatio(void) const
{
	return m_aspectRatio;
}

void Camera::SetClipAspectRatio(float aspect)
{
	m_aspectRatio = aspect;
}

float Camera::GetFovY(void) const
{
	return m_fovY;
}

float Camera::GetFovX(void) const
{
	return 2.0f * atan(m_aspectRatio * tan(m_fovY * 0.5f));
}

void Camera::UpdateProjection(void)
{
	
	float w = 1.0f / tan(GetFovX() * 0.5f);
	float h = 1.0f / tan(m_fovY * 0.5f);
	float Q = m_farClipDistance / (m_farClipDistance - m_nearClipDistance);

	m_projectionMatrix(0, 0) = w;
	m_projectionMatrix(1, 1) = h;
	m_projectionMatrix(2, 2) = Q;
	m_projectionMatrix(3, 2) = - Q * m_nearClipDistance;
	m_projectionMatrix(2, 3) = 1;

}