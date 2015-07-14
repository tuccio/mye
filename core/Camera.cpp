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

	SetProjection(90.0f, 1.0f, 0.1f, 100.0f);

	m_viewMatrixUptodate       = false;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;

}


Camera::~Camera(void)
{
}

/* View */

void Camera::LookAt(const Vector3 & position,
                    const Vector3 & up,
                    const Vector3 & target)
{



	Vector3 z = (target - position).Normalize();
	Vector3 x = up.Cross(z).Normalize();
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

void Camera::SetOrientation(const Quaternion & direction)
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

void Camera::SetPosition(const Vector3 & position)
{
	m_position           = position;
	m_viewMatrixUptodate = false;
	m_frustumUptodate    = false;
}

Vector3 & Camera::Position(void)
{
	return m_position;
}

void Camera::Pitch(Real angle)
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

void Camera::Yaw(Real angle)
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

void Camera::Roll(Real angle)
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

void Camera::SetProjection(Real fovy, Real aspect, Real zNear, Real zFar)
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

void Camera::SetNearClipDistance(Real near)
{
	m_nearClipDistance         = near;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

mye::math::Real Camera::GetFarClipDistance(void) const
{
	return m_farClipDistance;
}

void Camera::SetFarClipDistance(Real far)
{
	m_farClipDistance          = far;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

void Camera::SetClipDistances(Real near, Real far)
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

void Camera::SetClipAspectRatio(Real aspect)
{
	m_aspectRatio              = aspect;
	m_projectionMatrixUptodate = false;
	m_frustumUptodate          = false;
}

mye::math::Real Camera::GetFovY(void) const
{
	return Degrees(m_fovY);
}

void Camera::SetFovY(Real fovy)
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

Ray Camera::RayCast(const Vector2 & screenCoords) const
{

	Matrix4 invViewProjMatrix = (GetProjectionMatrix() * GetViewMatrix()).Inverse();

	Vector2 deviceCoords(2 * screenCoords.x() - 1,
						 1 - 2 * screenCoords.y());

	Vector4 nearH = invViewProjMatrix * Vector4(deviceCoords.xy(), 0, 1);
	Vector4 farH  = invViewProjMatrix * Vector4(deviceCoords.xy(), 1, 1);

	Vector3 near = nearH.xyz() / nearH.www();
	Vector3 far  =  farH.xyz() /  farH.www();

	return Ray(m_position, (far - near).Normalize());

}

Matrix4 Camera::GetViewMatrix(void) const
{

	if (m_viewMatrixUptodate)
	{
		return m_viewMatrix;
	}

	Matrix3 R  = RotationMatrix3(m_orientation);
	Matrix3 Rt = R.Transpose();
	Vector3 t  = - (Rt * m_position);

	Matrix4 viewMatrix(Rt);

	viewMatrix.m03() = t.x();
	viewMatrix.m13() = t.y();
	viewMatrix.m23() = t.z();

	return viewMatrix;

}

Matrix4 Camera::GetProjectionMatrix(void) const
{

	if (m_projectionMatrixUptodate)
	{
		return m_projectionMatrix;
	}

	Real tanFovY  = Tangent(m_fovY * 0.5f);
	Real yScale   = 1.f / tanFovY;
	Real xScale   = yScale / m_aspectRatio;
	Real invDepth = 1.f / (m_farClipDistance - m_nearClipDistance);

	Real Q = m_farClipDistance * invDepth;

	Matrix4 projectionMatrix(0);

	projectionMatrix.m00() = xScale;
	projectionMatrix.m11() = yScale;
	projectionMatrix.m22() = Q;
	projectionMatrix.m23() = - Q * m_nearClipDistance;
	projectionMatrix.m32() = 1;

	return projectionMatrix;

}