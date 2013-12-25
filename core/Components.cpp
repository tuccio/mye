#include "Components.h"

using namespace mye::core;

const char* COMPONENT_NAMES[Components::COMPONENTS_N] = {
	NullComponent::Name(),
	VariableComponent::Name(),
	TransformComponent::Name()
};