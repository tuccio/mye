#include "VariableComponent.h"

using namespace mye::core;

/* Transform specialization */
/*

VariableComponent<Transform>::VariableComponent(const String &name) :
	Component(ComponentTypes::VARIABLE_COMPONENT,
	name)
{
}

VariableComponent<Transform>::~VariableComponent(void)
{
}

VariableComponent<Transform>* VariableComponent<Transform>::Clone(void) const
{
	return new VariableComponent<Transform>(*this);
}

const Transform& VariableComponent<Transform>::Get(void) const
{
	return m_transform;
}

void VariableComponent<Transform>::Set(const Transform &t)
{
	m_transform = t;
}

std::type_index VariableComponent<Transform>::GetVariableType(void) const
{
	return typeid(Transform);
}*/