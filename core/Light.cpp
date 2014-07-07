#include "Light.h"

using namespace mye::core;

Light::Light(void)
{
}

Light::Light(
	LightType type,
	const mye::math::Vector3 &color,
	const mye::math::Vector3 &intensity,
	const mye::math::Vector3 &position,
	const mye::math::Vector3 &direction,
	mye::math::Real spotAngle,
	mye::math::Real range) :
	m_type(type),
	m_color(color),
	m_intensity(intensity),
	m_position(position),
	m_direction(direction),
	m_spotAngle(spotAngle),
	m_range(range)
{

}

Light::Light(const PointLight &pointlight) :
	m_type(LightType::POINTLIGHT),
	m_color(pointlight.color),
	m_intensity(pointlight.intensity),
	m_position(pointlight.position),
	m_range(pointlight.range)
{
}


Light::~Light(void)
{
}

LightType Light::GetType(void) const
{
	return m_type;
}

void Light::SetType(LightType type)
{
	m_type = type;
}

const mye::math::Vector3& Light::GetColor(void) const
{
	return m_color;
}

void Light::SetColor(const mye::math::Vector3 &color)
{
	m_color = color;
}

const mye::math::Vector3& Light::GetIntensity(void) const
{
	return m_intensity;
}

void Light::SetIntensity(const mye::math::Vector3 &intensity)
{
	m_intensity = intensity;
}

const mye::math::Vector3& Light::GetPosition(void) const
{
	return m_position;
}

void Light::SetPosition(const mye::math::Vector3 &position)
{
	m_position = position;
}

const mye::math::Vector3& Light::GetDirection(void) const
{
	return m_direction;
}

void Light::SetDirection(const mye::math::Vector3 &direction)
{
	m_direction = direction;
}

mye::math::Real Light::GetSpotAngle(void) const
{
	return m_spotAngle;
}

void Light::SetSpotAngle(mye::math::Real spotAngle)
{
	m_spotAngle = spotAngle;
}

mye::math::Real Light::GetRange(void) const
{
	return m_range;
}

void Light::SetRange(mye::math::Real range)
{
	m_range = range;
}