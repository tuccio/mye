#include "MouseKeyboardInput.h"
#include <mye/core/Game.h>

using namespace mye::win;
using namespace mye::core;

NativeWindowsInput::NativeWindowsInput(void)
{
}


NativeWindowsInput::~NativeWindowsInput(void)
{
}

bool NativeWindowsInput::Init(void)
{
	return m_keyboard.Hook() && m_mouse.Hook();
}

void NativeWindowsInput::ShutDown(void)
{
	m_keyboard.Unhook();
	m_mouse.Unhook();
}

bool NativeWindowsInput::HasKeyboard(void) const
{
	return true;
}

WinKeyboard * NativeWindowsInput::GetKeyboard(void)
{
	return &m_keyboard;
}

const WinKeyboard * NativeWindowsInput::GetKeyboard(void) const
{
	return &m_keyboard;
}

bool NativeWindowsInput::HasMouse(void) const
{
	return true;
}

WinMouse * NativeWindowsInput::GetMouse(void)
{
	return &m_mouse;
}
const Mouse * NativeWindowsInput::GetMouse(void) const
{
	return &m_mouse;
}

void NativeWindowsInput::Preupdate(void)
{

	InputModule::Preupdate();

}