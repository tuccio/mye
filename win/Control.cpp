#include "Control.h"

using namespace mye::win;

Control::Control(ControlType type)
{
	m_id = IDGenerator::GetSingleton().Generate();
	m_controlType = type;
}


Control::~Control(void)
{
}

void Control::Destroy(void)
{

}

void Control::Show(void)
{

}

void Control::Hide(void)
{

}

void Control::SetPosition(const mye::math::Vector2i &position)
{

}

void Control::SetSize(const mye::math::Vector2i &size)
{

}

bool Control::Notify(unsigned int code, LPARAM lParam)
{
	return false;
}

IDGenerator::ID Control::GetID(void) const
{
	return m_id;
}

ControlType Control::GetControlType(void) const
{
	return m_controlType;
}