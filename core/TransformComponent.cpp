#include "TransformComponent.h"

using namespace mye::core;

TransformComponent::TransformComponent(void) :
	VariableComponent<Transform>("transform")
{
	m_type = TRANSFORM_COMPONENT;
}


TransformComponent::~TransformComponent(void)
{
}
