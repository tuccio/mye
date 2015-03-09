#include "MouseComponent.h"


#include "Game.h"

using namespace mye::core;

MouseComponent::MouseComponent(void) :
	Component(ComponentTypes::MOUSE, "mouse")
{
}


MouseComponent::~MouseComponent(void)
{
}

MouseComponent * MouseComponent::Clone(void) const
{
	return new MouseComponent(*this);
}

void MouseComponent::OnAttach(GameObject * gameObject)
{
	Component::OnAttach(gameObject);
	Game::GetSingleton().GetInputModule()->GetMouse()->AddListener(static_cast<MouseListener*>(this));
}

void MouseComponent::OnDetach(void)
{
	Component::OnDetach();
	Game::GetSingleton().GetInputModule()->GetMouse()->RemoveListener(static_cast<MouseListener*>(this));
}

void MouseComponent::OnMouseKeyPress(MouseVK key)
{

	if (m_owner)
	{
		ScriptMessage msg = { SCRIPT_MESSAGE_MOUSE_PRESSED, static_cast<unsigned int>(key), m_owner->GetHandle() };
		Game::GetSingleton().GetScriptModule()->QueueMessage(msg);
	}

}

void MouseComponent::OnMouseKeyRelease(MouseVK key, FloatSeconds time)
{

	if (m_owner)
	{
		ScriptMessage msg = { SCRIPT_MESSAGE_MOUSE_RELEASED, static_cast<unsigned int>(key), m_owner->GetHandle() };
		msg.data.f = time;
		Game::GetSingleton().GetScriptModule()->QueueMessage(msg);
	}

}

void MouseComponent::OnMouseKeyHold(MouseVK key, FloatSeconds time)
{

	if (m_owner)
	{
		ScriptMessage msg = { SCRIPT_MESSAGE_MOUSE_HELD, static_cast<unsigned int>(key), m_owner->GetHandle() };
		msg.data.f = time;
		Game::GetSingleton().GetScriptModule()->QueueMessage(msg);
	}

}

void MouseComponent::OnMouseMove(const mye::math::Vector2 & from, const mye::math::Vector2 & to)
{

	if (m_owner)
	{

		ScriptMessage msg = { SCRIPT_MESSAGE_MOUSE_MOVED, 0, m_owner->GetHandle() };

		msg.data.f4[0] = from.x();
		msg.data.f4[1] = from.y();

		msg.data.f4[2] = to.x();
		msg.data.f4[3] = to.y();

		Game::GetSingleton().GetScriptModule()->QueueMessage(msg);

	}

}