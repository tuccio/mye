#include "Camera.h"

#include <cassert>
#include <cmath>
#include <Eigen/Geometry>

#include <mye/math/Trigonometry.h>

using namespace mye::core;
using namespace mye::math;

Camera::Camera(void) :
	m_orientation(1.0f, 0.0f, 0.0f, 0.0f),
	m_position(0.0f, 0.0f, 0.0f)
{

	m_nearClipDistance = 1.0f;
	m_farClipDistance = 100.0f;

	m_viewMatrix = Eigen::Matrix4f::Identity();
	m_projectionMatrix = Eigen::Matrix4f::Zero();

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

	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Eigen::Quaternionf pitchQuaternion(cosHalfAngle,
		sinHalfAngle,
		0,
		0);

	m_orientation = m_orientation * pitchQuaternion;

}

void Camera::Yaw(float angle)
{

	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Eigen::Quaternionf yawQuaternion(cosHalfAngle,
		0,
		sinHalfAngle,
		0);

	m_orientation = m_orientation * yawQuaternion;

}

void Camera::Roll(float angle)
{

	float cosHalfAngle = Cosine(Radians(angle) * 0.5f);
	float sinHalfAngle = Sine(Radians(angle) * 0.5f);

	Eigen::Quaternionf rollQuaternion(cosHalfAngle,
		0,
		0,
		sinHalfAngle);

	m_orientation = m_orientation * rollQuaternion;

}

void Camera::UpdateView(void)
{
	
	float twoXSquared = 2 * m_orientation.x() * m_orientation.x();
	float twoYSquared = 2 * m_orientation.y() * m_orientation.y();
	float twoZSquared = 2 * m_orientation.z() * m_orientation.z();
	float twoXY = 2 * m_orientation.x() * m_orientation.y();
	float twoWZ = 2 * m_orientation.w() * m_orientation.z();
	float twoXZ = 2 * m_orientation.x() * m_orientation.z();
	float twoWY = 2 * m_orientation.w() * m_orientation.y();
	float twoYZ = 2 * m_orientation.y() * m_orientation.z();
	float twoWX = 2 * m_orientation.w() * m_orientation.x();

	m_viewMatrix(0, 0) = 1 - twoYSquared - twoZSquared;
	m_viewMatrix(0, 2) = twoXZ + twoWY;
	m_viewMatrix(0, 1) = twoXY - twoWZ;
	m_viewMatrix(0, 3) = - Eigen::Vector3f(m_viewMatrix(0, 0),
		m_viewMatrix(0, 1),
		m_viewMatrix(0, 2)).dot(m_position);

	m_viewMatrix(1, 0) = twoXY + twoWZ;
	m_viewMatrix(1, 1) = 1 - twoXSquared - twoZSquared;
	m_viewMatrix(1, 2) = twoYZ - twoWX;
	m_viewMatrix(1, 3) = - Eigen::Vector3f(m_viewMatrix(1, 0),
		m_viewMatrix(1, 1),
		m_viewMatrix(1, 2)).dot(m_position);

	m_viewMatrix(2, 0) = twoXZ - twoWY;
	m_viewMatrix(2, 1) = twoYZ + twoWX;
	m_viewMatrix(2, 2) = 1 - twoXSquared - twoYSquared;
	m_viewMatrix(2, 3) = - Eigen::Vector3f(m_viewMatrix(2, 0),
		m_viewMatrix(2, 1),
		m_viewMatrix(2, 2)).dot(m_position);

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
	m_projectionMatrix(2, 3) = - m_nearClipDistance * m_farClipDistance * invDepth;
	m_projectionMatrix(3, 2) = 1;

}