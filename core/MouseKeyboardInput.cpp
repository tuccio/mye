#include "MouseKeyboardInput.h"
#include "Game.h"

using namespace mye::core;

MouseKeyboardInput::MouseKeyboardInput(void)
{
}


MouseKeyboardInput::~MouseKeyboardInput(void)
{
}

bool MouseKeyboardInput::Init(void)
{
	IWindow *window = Game::GetSingleton().GetGraphicsModule()->GetWindow();
	window->AttachKeyboard(&m_keyboard);
	window->AttachMouse(&m_mouse);
	return true;
}

void MouseKeyboardInput::ShutDown(void)
{
	IWindow *window = Game::GetSingleton().GetGraphicsModule()->GetWindow();
	window->AttachKeyboard(NULL);
	window->AttachMouse(NULL);
}

bool MouseKeyboardInput::HasKeyboard(void) const
{
	return true;
}

Keyboard* MouseKeyboardInput::GetKeyboard(void)
{
	return &m_keyboard;
}

const Keyboard* MouseKeyboardInput::GetKeyboard(void) const
{
	return &m_keyboard;
}

bool MouseKeyboardInput::HasMouse(void) const
{
	return true;
}

Mouse* MouseKeyboardInput::GetMouse(void)
{
	return &m_mouse;
}
const Mouse* MouseKeyboardInput::GetMouse(void) const
{
	return &m_mouse;
}