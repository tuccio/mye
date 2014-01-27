#include "InputModule.h"

using namespace mye::core;

bool InputModule::HasKeyboard(void) const
{
	return false;
}

Keyboard* InputModule::GetKeyboard(void)
{
	return NULL;
}

const Keyboard* InputModule::GetKeyboard(void) const
{
	return NULL;
}

bool InputModule::HasMouse(void) const
{
	return false;
}

Mouse* InputModule::GetMouse(void)
{
	return NULL;
}

const Mouse* InputModule::GetMouse(void) const
{
	return NULL;
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