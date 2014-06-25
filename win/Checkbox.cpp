#include "Checkbox.h"
#include "Utils.h"

#include <CommCtrl.h>
#include <windowsx.h>

using namespace mye::win;
using namespace mye::math;

Checkbox::Checkbox(void) :
	Control(CT_CHECKBOX)
{
}


Checkbox::~Checkbox(void)
{
}

void Checkbox::Create(Window &parent,
					  const std::string &text, 
					  const Vector2i &position,
					  const Vector2i &size)
{

	m_hCheckbox = CreateWindow( 
		WC_BUTTON,
		text.c_str(),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		position.x(),
		position.y(),
		size.x(),
		size.y(),
		parent.GetHandle(),
		(HMENU) m_id,
		nullptr,
		nullptr);

	SendMessage(m_hCheckbox, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), (LPARAM) TRUE);

	if (!m_hCheckbox)
	{
		ShowErrorBox();
	}

}

void Checkbox::Destroy(void)
{
	DestroyWindow(m_hCheckbox);
	m_hCheckbox = nullptr;
}

bool Checkbox::IsChecked(void) const
{
	return SendMessage(m_hCheckbox, BM_GETCHECK, 0, 0) ? true : false;
}

void Checkbox::SetCheck(bool check)
{
	SendMessage(m_hCheckbox, BM_SETCHECK, (check ? BST_CHECKED : BST_UNCHECKED), 0x0);
}

void Checkbox::Show(void)
{
	ShowWindow(m_hCheckbox, SW_SHOW);
}

void Checkbox::Hide(void)
{
	ShowWindow(m_hCheckbox, SW_HIDE);
}

void Checkbox::SetPosition(const mye::math::Vector2i &position)
{
	SetWindowPos(m_hCheckbox,
		nullptr,
		position.x(),
		position.y(),
		0,
		0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
}

void Checkbox::SetSize(const mye::math::Vector2i &size)
{
	SetWindowPos(m_hCheckbox,
		0,
		0,
		0,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
}