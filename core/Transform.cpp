#include "Transform.h"

using namespace mye::core;

const Transform Transform::IDENTITY(mye::math::Vector3f(1, 1, 1),
									mye::math::Quaternionf(1, 0, 0, 0),
									mye::math::Vector3f(0, 0, 0));

const Transform& Transform::Identity(void)
{
	return IDENTITY;
}

Transform::Transform(void)
{
}

Transform::Transform(const mye::math::Vector3f &scale,
					 const mye::math::Quaternionf &rotation,
					 const mye::math::Vector3f &translation) :
m_scale(scale),
	m_rotation(rotation),
	m_translation(translation)
{
}

Transform::~Transform(void)
{
}

mye::math::Matrix4f Transform::GetMatrix(void) const
{

	mye::math::Matrix4f S = mye::math::Matrix4f(1.0f);
	S(0, 0) = m_scale.x();
	S(1, 1) = m_scale.y();
	S(2, 2) = m_scale.z();

	mye::math::Matrix4f R = RotationMatrix4(m_rotation);

	mye::math::Matrix4f T = mye::math::Matrix4f(1.0f);
	T(0, 3) = m_translation.x();
	T(1, 3) = m_translation.y();
	T(2, 3) = m_translation.z();

	//return S * R * T;
	return T * R * S;
}

const mye::math::Vector3f& Transform::GetScale(void) const
{
	return m_scale;
}

void Transform::SetScale(const mye::math::Vector3f &scale)
{
	m_scale = scale;
}

const mye::math::Quaternionf& Transform::GetRotation(void) const
{
	return m_rotation;
}

void Transform::SetRotation(const mye::math::Quaternionf &rotation)
{
	m_rotation = rotation;
}

const mye::math::Vector3f& Transform::GetTranslation(void) const
{
	return m_translation;
}

void Transform::SetTranslation(const mye::math::Vector3f &translation)
{
	m_translation = translation;
}