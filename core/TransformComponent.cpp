#include "TransformComponent.h"

using namespace mye::core;
using namespace mye::math;

#define __MYE_TRANSFORM_INVALIDATE_MATRIX() m_matrix.m33() = -1;
#define __MYE_TRANSFORM_IS_VALID() (m_matrix.m33() == 1)

TransformComponent::TransformComponent(void) :
	Component(ComponentTypes::TRANSFORM, "transform"),
	m_transform(Transformf::Identity()),
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

const Vector3f& TransformComponent::GetPosition(void) const
{
	return m_transform.GetPosition();
}

void TransformComponent::SetPosition(const Vector3f &position)
{
	m_transform.SetPosition(position);
	__MYE_TRANSFORM_INVALIDATE_MATRIX();
}

const Vector3f& TransformComponent::GetScale(void) const
{
	return m_transform.GetScale();
}

void TransformComponent::SetScale(const Vector3f &scale)
{
	m_transform.SetScale(scale);
	__MYE_TRANSFORM_INVALIDATE_MATRIX();
}

const Quaternionf& TransformComponent::GetOrientation(void) const
{
	return m_transform.GetOrientation();
}

void TransformComponent::SetOrientation(const Quaternionf &orientation)
{
	m_transform.SetOrientation(orientation);
	__MYE_TRANSFORM_INVALIDATE_MATRIX();
}

const Matrix4f& TransformComponent::GetWorldMatrix(void)
{

	if (!__MYE_TRANSFORM_IS_VALID())
	{
		m_matrix = m_transform.GetTRSMatrix();
	}

	return m_matrix;

}

Vector3f TransformComponent::Up(void) const
{
	return m_transform.GetOrientation().Rotate(Vector3f(0, 1, 0));
}

Vector3f TransformComponent::Right(void) const
{
	return m_transform.GetOrientation().Rotate(Vector3f(1, 0, 0));
}

Vector3f TransformComponent::Forward(void) const
{
	return m_transform.GetOrientation().Rotate(Vector3f(0, 0, 1));
}