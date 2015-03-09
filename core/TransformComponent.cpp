#include "TransformComponent.h"

using namespace mye::core;
using namespace mye::math;

#define __MYE_TRANSFORM_UPDATE_NOTIFICATION() { (m_matrix).m33() = -1; }
#define __MYE_TRANSFORM_MATRIX_IS_VALID(matrix) ((matrix).m33() == 1)
#define __MYE_TRANSFORM_MATRIX_VALIDATE(matrix) ((matrix).m33() = 1)
#define __MYE_TRANSFORM_MATRIX_INVALIDATE(matrix) ((matrix).m33() = -1)

TransformComponent::TransformComponent(void) :
	Component(ComponentTypes::TRANSFORM, "transform"),
	m_transform(Transform::Identity())
{
}

TransformComponent::TransformComponent(const Vector3 & position,
									   const Quaternion & orientation,
									   const Vector3 & scale) :
	Component(ComponentTypes::TRANSFORM, "transform"),
	m_transform(position, orientation, scale) { }

TransformComponent::~TransformComponent(void)
{
}

TransformComponent * TransformComponent::Clone(void) const
{
	return new TransformComponent(*this);
}

Matrix4 TransformComponent::GetWorldMatrix(void)
{
	return m_transform.GetSRTMatrix();
}