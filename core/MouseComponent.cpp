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
	
	MYE_EVENT_MANAGER_ADD_LISTENER(this,
	                               EventType::MOUSE_KEY_PRESS,
	                               EventType::MOUSE_KEY_RELEASE,
	                               EventType::MOUSE_KEY_HOLD,
	                               EventType::MOUSE_MOVE);

}

void MouseComponent::OnDetach(void)
{
	Component::OnDetach();

	MYE_EVENT_MANAGER_REMOVE_LISTENER(this,
	                                  EventType::MOUSE_KEY_PRESS,
	                                  EventType::MOUSE_KEY_RELEASE,
	                                  EventType::MOUSE_KEY_HOLD,
	                                  EventType::MOUSE_MOVE);

}

void MouseComponent::OnEvent(const IEvent * e)
{

	if (m_owner)
	{
		Game::GetSingleton().GetScriptModule()->OnEvent(m_owner, e);
	}

}