#include "BehaviourComponent.h"

#include "GameObject.h"
#include "Utils.h"
#include "Game.h"

using namespace mye::core;

BehaviourComponent::BehaviourComponent(void) :
	Component(ComponentTypes::BEHAVIOUR, "behaviour")
{
}

BehaviourComponent::BehaviourComponent(const ScriptPointer & s) :
	Component(ComponentTypes::BEHAVIOUR, "behaviour"),
	m_script(s)
{
}


BehaviourComponent::~BehaviourComponent(void)
{
}

BehaviourComponent * BehaviourComponent::Clone(void) const
{
	return new BehaviourComponent(*this);
}