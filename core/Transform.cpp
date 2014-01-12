#include "Transform.h"

using namespace mye::core;

const Transform Transform::IDENTITY(Eigen::Vector3f::Ones(),
									Eigen::Quaternionf::Identity(),
									Eigen::Vector3f::Zero());

const Transform& Transform::Identity(void)
{
	return IDENTITY;
}

Transform::Transform(void)
{
}

Transform::Transform(const Eigen::Vector3f &scale,
					 const Eigen::Quaternionf &rotation,
					 const Eigen::Vector3f &translation) :
m_scale(scale),
	m_rotation(rotation),
	m_translation(translation)
{
}

Transform::~Transform(void)
{
}

Eigen::Matrix4f Transform::GetMatrix(void) const
{

	Eigen::Matrix4f S = Eigen::Matrix4f::Identity();
	S(0, 0) = m_scale.x();
	S(1, 1) = m_scale.y();
	S(2, 2) = m_scale.z();

	Eigen::Matrix4f R = Eigen::Matrix4f::Identity();
	
	R.block<3, 3>(0, 0) = m_rotation.matrix();

	Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
	T(3, 0) = m_translation.x();
	T(3, 1) = m_translation.y();
	T(3, 2) = m_translation.z();

	return S * R * T;
}

const Eigen::Vector3f& Transform::GetScale(void) const
{
	return m_scale;
}

void Transform::SetScale(const Eigen::Vector3f &scale)
{
	m_scale = scale;
}

const Eigen::Quaternionf& Transform::GetRotation(void) const
{
	return m_rotation;
}

void Transform::SetRotation(const Eigen::Quaternionf &rotation)
{
	m_rotation = rotation;
}

const Eigen::Vector3f& Transform::GetTranslation(void) const
{
	return m_translation;
}

void Transform::SetTranslation(const Eigen::Vector3f &translation)
{
	m_translation = translation;
}