#include "TransformComponent.h"

using namespace mye::core;
using namespace mye::math;

#define __MYE_TRANSFORM_UPDATE_NOTIFICATION() { (m_matrix).m33() = -1; m_changed = true; }
#define __MYE_TRANSFORM_MATRIX_IS_VALID(matrix) ((matrix).m33() == 1)
#define __MYE_TRANSFORM_MATRIX_VALIDATE(matrix) ((matrix).m33() = 1)

TransformComponent::TransformComponent(void) :
	Component(ComponentTypes::TRANSFORM, "transform"),
	m_transform(Transform::Identity()),
	m_matrix(1.0f)
{
}


TransformComponent::~TransformComponent(void)
{
}

TransformComponent* TransformComponent::Clone(void) const
{
	return new TransformComponent(*this);
}

const Vector3& TransformComponent::GetPosition(void) const
{
	return m_transform.GetPosition();
}

void TransformComponent::SetPosition(const Vector3 &position)
{
	m_transform.SetPosition(position);
	__MYE_TRANSFORM_UPDATE_NOTIFICATION();
}

const Vector3& TransformComponent::GetScale(void) const
{
	return m_transform.GetScale();
}

void TransformComponent::SetScale(const Vector3 &scale)
{
	m_transform.SetScale(scale);
	__MYE_TRANSFORM_UPDATE_NOTIFICATION();
}

const Quaternion& TransformComponent::GetOrientation(void) const
{
	return m_transform.GetOrientation();
}

void TransformComponent::SetOrientation(const Quaternion &orientation)
{
	m_transform.SetOrientation(orientation);
	__MYE_TRANSFORM_UPDATE_NOTIFICATION();
}

const Matrix4& TransformComponent::GetWorldMatrix(void)
{

	if (!__MYE_TRANSFORM_MATRIX_IS_VALID(m_matrix))
	{
		m_matrix = m_transform.GetSRTMatrix();
	}

	return m_matrix;

}

Vector3 TransformComponent::Up(void) const
{
	return m_transform.GetOrientation().Rotate(Vector3(0, 1, 0));
}

Vector3 TransformComponent::Right(void) const
{
	return m_transform.GetOrientation().Rotate(Vector3(1, 0, 0));
}

Vector3 TransformComponent::Forward(void) const
{
	return m_transform.GetOrientation().Rotate(Vector3(0, 0, 1));
}

void TransformComponent::Preupdate(void)
{
	m_changed = false;
}

bool TransformComponent::Postupdate(mye::math::Matrix4 &oldTransform)
{

	if (m_changed)
	{
		oldTransform = m_matrix;
		__MYE_TRANSFORM_MATRIX_VALIDATE(oldTransform);
	}

	return m_changed;

}