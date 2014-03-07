#include "BehaviourComponent.h"

#include "GameObject.h"

using namespace mye::core;

BehaviourComponent::BehaviourComponent(void) :
	Component(ComponentTypes::BEHAVIOUR, "behaviour")
{
}

BehaviourComponent::BehaviourComponent(const mye::lua::BehaviourScriptPointer &s) :
	Component(ComponentTypes::BEHAVIOUR, "behaviour"),
	m_script(s)
{
}


BehaviourComponent::~BehaviourComponent(void)
{
}

BehaviourComponent* BehaviourComponent::Clone(void) const
{
	return new BehaviourComponent(*this);
}

void BehaviourComponent::Init(void)
{
	m_script->Call<void, GameObjectHandle>("Init", m_owner->GetHandle());
}

void BehaviourComponent::Update(FloatSeconds dt)
{
	m_script->Call<void, GameObjectHandle, float>("Update", m_owner->GetHandle(), dt);
}