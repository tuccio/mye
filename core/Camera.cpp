#include "Camera.h"

using namespace mye::core;

Camera::Camera(void) :
	m_orientation(1.0f, 0.0f, 0.0f, 0.0f),
	m_position(0.0f, 0.0f, 0.0f)
{
	m_nearClipDistance = 1.0f;
	m_farClipDistance = 100.0f;
}


Camera::~Camera(void)
{
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

float Camera::GetNearClipDistance(void) const
{
	return m_nearClipDistance;
}

void Camera::SetNearClipDistance(float nearClipDistance)
{
	m_nearClipDistance = nearClipDistance;
}

float Camera::GetFarClipDistance(void) const
{
	return m_farClipDistance;
}

void Camera::SetFarClipDistance(float farClipDistance)
{
	m_farClipDistance = farClipDistance;
}