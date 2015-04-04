#include "LightComponent.h"
#include "ComponentTypes.h"

#include "GameObject.h"

using namespace mye::core;
using namespace mye::math;

LightComponent::LightComponent(void) :
	Component(ComponentTypes::LIGHT, "light")
{
}

LightComponent::LightComponent(const Light &light) :
	Component(ComponentTypes::LIGHT, "light"),
	Light(light)
{
}

LightComponent::~LightComponent(void)
{
}

LightComponent * LightComponent::Clone(void) const
{
	return new LightComponent(*this);
}

mye::math::Matrix4 LightComponent::GetWorldMatrix(void) const
{
	return m_owner->GetTransformComponent()->GetWorldMatrix();
}