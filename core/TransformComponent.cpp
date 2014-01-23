#include "TransformComponent.h"

using namespace mye::core;
using namespace mye::math;

TransformComponent::TransformComponent(void) :
	VariableComponent<Transformf>("transform")
{
	m_type = TRANSFORM_COMPONENT;
}


TransformComponent::~TransformComponent(void)
{
}
