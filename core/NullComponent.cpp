#include "NullComponent.h"

using namespace mye::core;

NullComponent NullComponent::dummy;

const char* NullComponent::Name(void)
{
	return "null";
}

NullComponent::NullComponent(void) :
	Component(Name())
{
}


NullComponent::~NullComponent(void)
{
}

bool NullComponent::IsNull(void) const
{
	return true;
}

Cloneable* NullComponent::Clone(void) const
{
	return new NullComponent;
}

Component& NullComponent::operator= (const Component& component)
{
	return *this;
}