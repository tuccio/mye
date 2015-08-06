#include "Light.h"

using namespace mye::core;
using namespace mye::math;

Light::Light(void)
{
}

Light::Light(LightType type,
             const Vector3 & color,
             Real intensity,
             const Vector3 & position,
             const Vector3 & direction,
             Real spotAngle,
             Real range) :
	m_type(type),
	m_color(color),
	m_intensity(intensity),
	m_position(position),
	m_direction(direction),
	m_spotAngle(spotAngle),
	m_range(range)
{

}

Light::Light(const PointLight & pointlight) :
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

const Vector3 & Light::GetColor(void) const
{
	return m_color;
}

void Light::SetColor(const Vector3 & color)
{
	m_color = color;
}

Real Light::GetIntensity(void) const
{
	return m_intensity;
}

void Light::SetIntensity(Real intensity)
{
	m_intensity = intensity;
}

const Vector3 & Light::GetPosition(void) const
{
	return m_position;
}

void Light::SetPosition(const Vector3 & position)
{
	m_position = position;
}

const Vector3 & Light::GetDirection(void) const
{
	return m_direction;
}

void Light::SetDirection(const Vector3 & direction)
{
	m_direction = direction;
}

Real Light::GetSpotAngle(void) const
{
	return m_spotAngle;
}

void Light::SetSpotAngle(Real spotAngle)
{
	m_spotAngle = spotAngle;
}

Real Light::GetRange(void) const
{
	return m_range;
}

void Light::SetRange(Real range)
{
	m_range = range;
}