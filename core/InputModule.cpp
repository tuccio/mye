#include "InputModule.h"

using namespace mye::core;

bool InputModule::HasKeyboard(void) const
{
	return false;
}

Keyboard* InputModule::GetKeyboard(void)
{
	return nullptr;
}

const Keyboard* InputModule::GetKeyboard(void) const
{
	return nullptr;
}

bool InputModule::HasMouse(void) const
{
	return false;
}

Mouse* InputModule::GetMouse(void)
{
	return nullptr;
}

const Mouse* InputModule::GetMouse(void) const
{
	return nullptr;
}

void InputModule::ResetDeltas(void)
{

	Mouse *mouse = GetMouse();

	if (mouse)
	{
		mouse->ResetDelta();
		mouse->SetWheelDelta(0);
	}

}