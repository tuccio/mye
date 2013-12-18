#include "TransformComponent.h"

using namespace mye::core;

const char* TransformComponent::Name(void)
{
	return "transform";
}

TransformComponent::TransformComponent(void) :
	Component(Name())
{
}


TransformComponent::~TransformComponent(void)
{
}

Cloneable* TransformComponent::Clone(void) const
{
	TransformComponent *tc = new TransformComponent;
	*tc = *this;
	return tc;
}

Component& TransformComponent::operator= (const Component& component)
{
	*this = static_cast<const TransformComponent&>(component);
	return *this;
}