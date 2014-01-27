#include "MouseKeyboardInput.h"
#include <mye/core/Game.h>

using namespace mye::win;
using namespace mye::core;

MouseKeyboardInput::MouseKeyboardInput(void)
{
}


MouseKeyboardInput::~MouseKeyboardInput(void)
{
}

bool MouseKeyboardInput::Init(void)
{

	// 	IWindow *window = Game::GetSingleton().GetGraphicsModule()->GetWindow();
	// 	window->AttachKeyboard(&m_keyboard);
	// 	window->AttachMouse(&m_mouse);

	return m_keyboard.Hook() && m_mouse.Hook();
}

void MouseKeyboardInput::ShutDown(void)
{
// 	IWindow *window = Game::GetSingleton().GetGraphicsModule()->GetWindow();
// 	window->AttachKeyboard(NULL);
// 	window->AttachMouse(NULL);
	m_keyboard.Unhook();
}

bool MouseKeyboardInput::HasKeyboard(void) const
{
	return true;
}

WinKeyboard* MouseKeyboardInput::GetKeyboard(void)
{
	return &m_keyboard;
}

const WinKeyboard* MouseKeyboardInput::GetKeyboard(void) const
{
	return &m_keyboard;
}

bool MouseKeyboardInput::HasMouse(void) const
{
	return true;
}

WinMouse* MouseKeyboardInput::GetMouse(void)
{
	return &m_mouse;
}
const Mouse* MouseKeyboardInput::GetMouse(void) const
{
	return &m_mouse;
}

void MouseKeyboardInput::ResetDeltas(void)
{
	m_mouse.ResetDelta();
	m_mouse.SetWheelDelta(0);
}