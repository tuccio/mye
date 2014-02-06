#include "Button.h"
#include "Window.h"
#include "utils.h"

#include <CommCtrl.h>
#include <Windowsx.h>

using namespace mye::math;
using namespace mye::win;

Button::Button(void) :
	Control(CT_BUTTON)
{
}


Button::~Button(void)
{
}

void Button::Create(Window &parent,
					const std::string &text,
					const CallbackType &f,
					const mye::math::Vector2i &position,
					const mye::math::Vector2i &size)
{

	m_function = f;
	
	m_hButton = CreateWindow( 
		WC_BUTTON,
		text.c_str(),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		position.x(),
		position.y(),
		size.x(),
		size.y(),
		parent.GetHandle(),
		(HMENU) m_id,
		nullptr,
		nullptr);

	SendMessage(m_hButton, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), (LPARAM) TRUE);

	if (!m_hButton)
	{
		ShowErrorBox();
	}

}

void Button::Destroy(void)
{
	DestroyWindow(m_hButton);
	m_hButton = nullptr;
}

void Button::Show(void)
{
	ShowWindow(m_hButton, SW_SHOW);
}

void Button::Hide(void)
{
	ShowWindow(m_hButton, SW_HIDE);
}

void Button::SetPosition(const Vector2i &position)
{

	SetWindowPos(m_hButton,
		nullptr,
		position.x(),
		position.y(),
		nullptr,
		nullptr,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);

}

void Button::SetSize(const Vector2i &size)
{

	SetWindowPos(m_hButton,
		nullptr,
		nullptr,
		nullptr,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);

}

IDGenerator::ID Button::GetID(void) const
{
	return m_id;
}

std::string Button::GetText(void) const
{
	char buffer[256] = { 0 };
	Button_GetText(m_hButton, buffer, 255);
	return buffer;
}