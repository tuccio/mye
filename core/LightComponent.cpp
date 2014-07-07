#include "LightComponent.h"
#include "ComponentTypes.h"

using namespace mye::core;

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

LightComponent* LightComponent::Clone(void) const
{
	return new LightComponent(*this);
}