#include "KeyboardComponent.h"
#include "Game.h"

using namespace mye::core;

KeyboardComponent::KeyboardComponent(void) :
	Component(ComponentTypes::KEYBOARD, "keyboard")
{
}


KeyboardComponent::~KeyboardComponent(void)
{
}

KeyboardComponent * KeyboardComponent::Clone(void) const
{
	return new KeyboardComponent(*this);
}

void KeyboardComponent::OnAttach(GameObject * gameObject)
{

	Component::OnAttach(gameObject);

	MYE_EVENT_MANAGER_ADD_LISTENER(this,
	                               EventType::KEYBOARD_KEY_PRESS,
	                               EventType::KEYBOARD_KEY_RELEASE,
	                               EventType::KEYBOARD_KEY_HOLD);

}

void KeyboardComponent::OnDetach(void)
{

	Component::OnDetach();

	MYE_EVENT_MANAGER_REMOVE_LISTENER(this,
	                                  EventType::KEYBOARD_KEY_PRESS,
	                                  EventType::KEYBOARD_KEY_RELEASE,
	                                  EventType::KEYBOARD_KEY_HOLD);

}

void KeyboardComponent::OnEvent(const IEvent * e)
{

	if (m_owner)
	{
		Game::GetSingleton().GetScriptModule()->OnEvent(m_owner, e);
	}

}