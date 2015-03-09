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
	Game::GetSingleton().GetInputModule()->GetKeyboard()->AddListener(static_cast<KeyboardListener*>(this));
}

void KeyboardComponent::OnDetach(void)
{
	Component::OnDetach();
	Game::GetSingleton().GetInputModule()->GetKeyboard()->RemoveListener(static_cast<KeyboardListener*>(this));
}

void KeyboardComponent::OnKeyboardKeyPress(KeyboardVK key)
{

	if (m_owner)
	{
		ScriptMessage msg = { SCRIPT_MESSAGE_KEYBOARD_PRESSED, static_cast<unsigned int>(key), m_owner->GetHandle() };
		Game::GetSingleton().GetScriptModule()->QueueMessage(msg);
	}

}

void KeyboardComponent::OnKeyboardKeyRelease(KeyboardVK key, FloatSeconds time)
{

	if (m_owner)
	{
		ScriptMessage msg = { SCRIPT_MESSAGE_KEYBOARD_RELEASED, static_cast<unsigned int>(key), m_owner->GetHandle() };
		msg.data.f = time;
		Game::GetSingleton().GetScriptModule()->QueueMessage(msg);
	}

}

void KeyboardComponent::OnKeyboardKeyHold(KeyboardVK key, FloatSeconds time)
{

	if (m_owner)
	{
		ScriptMessage msg = { SCRIPT_MESSAGE_KEYBOARD_HELD, static_cast<unsigned int>(key), m_owner->GetHandle() };
		msg.data.f = time;
		Game::GetSingleton().GetScriptModule()->QueueMessage(msg);
	}

}