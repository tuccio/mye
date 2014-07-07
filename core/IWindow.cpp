#include "IWindow.h"

using namespace mye::core;

const IWindow::Properties IWindow::DefaultWindowProperties = {
	"mye Window",
	-1,
	-1,
	-1,
	-1,
	false
};

IWindow::IWindow(void) :
	m_keyboard(nullptr),
	m_mouse(nullptr)
{
}

IWindow::~IWindow(void)
{
}

void IWindow::AddListener(Listener *listener)
{
	m_listeners.push_back(listener);
}

void IWindow::RemoveListener(Listener *listener)
{

	auto it = std::find(
		m_listeners.begin(),
		m_listeners.end(),
		listener);

	if (it != m_listeners.end())
	{
		m_listeners.erase(it);
	}

}

void IWindow::ClearListeners(void)
{
	m_listeners.clear();
}

void IWindow::AttachKeyboard(Keyboard *keyboard)
{
	m_keyboard = keyboard;
}

void IWindow::AttachMouse(Mouse *mouse)
{
	m_mouse = mouse;
}

void IWindow::NotifyCreate(void)
{
	for (auto listener : m_listeners)
	{
		listener->OnCreate(this);
	}
}

void IWindow::NotifyDestroy(void)
{
	for (auto listener : m_listeners)
	{
		listener->OnDestroy(this);
	}
}

void IWindow::NotifyCommand(unsigned int id, unsigned int code)
{
	for (auto listener : m_listeners)
	{
		listener->OnCommand(id, code);
	}
}

void IWindow::NotifyResize(const mye::math::Vector2i &size)
{
	for (auto listener : m_listeners)
	{
		listener->OnResize(this, size);
	}
}

void IWindow::NotifyUserMessage(unsigned int msg,
								unsigned long intArg,
								long *ptrArg)
{
	for (auto listener : m_listeners)
	{
		listener->OnUserMessage(this, msg, intArg, ptrArg);
	}
}