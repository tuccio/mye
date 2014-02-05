#include "Edit.h"
#include "Utils.h"

#include <CommCtrl.h>
#include <windowsx.h>
#include <memory>

using namespace mye::win;

Edit::Edit(void) :
	Control(CT_EDIT),
	m_hEdit(NULL)
{
}


Edit::~Edit(void)
{
}

void Edit::Create(Window &parent,
				  const mye::math::Vector2i &position,
				  const mye::math::Vector2i &size)
{

	m_hEdit = CreateWindowEx(
		WS_EX_STATICEDGE,
		WC_EDIT,
		NULL,
		WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
		position.x(),
		position.y(),
		size.x(),
		size.y(),
		parent.GetHandle(),
		(HMENU) m_id,
		NULL,
		NULL);

	SendMessage(m_hEdit, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), (LPARAM) TRUE);

	if (!m_hEdit)
	{
		ShowErrorBox();
	}

}

void Edit::Destroy(void)
{
	DestroyWindow(m_hEdit);
	m_hEdit = NULL;
}

void Edit::SetText(const std::string text)
{
	Edit_SetText(m_hEdit, text.c_str());
}

std::string Edit::GetText(void) const
{
	size_t len = Edit_GetTextLength(m_hEdit);
	char *buffer = new char[len + 1];
	Edit_GetText(m_hEdit, buffer, len + 1);
	buffer[len] = '\0';
	std::string str(buffer);
	delete buffer;
	return str;
}

void Edit::SetReadOnly(bool readOnly)
{
	SendMessage(m_hEdit, EM_SETREADONLY, (readOnly ? TRUE : FALSE), 0x0);
}

void Edit::SetBorder(bool border)
{

	LONG dwExStyle = GetWindowLong(m_hEdit, GWL_EXSTYLE);

	if (border)
	{
		dwExStyle |= WS_EX_STATICEDGE;
	}
	else
	{
		dwExStyle &= ~WS_EX_STATICEDGE;
	}

	SetWindowLong(m_hEdit, GWL_EXSTYLE, dwExStyle);

	SetWindowPos(m_hEdit,
		NULL,
		0,
		0,
		0,
		0,
		SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOMOVE);

}

void Edit::SetPosition(const mye::math::Vector2i &position)
{
	SetWindowPos(m_hEdit,
		NULL,
		position.x(),
		position.y(),
		NULL,
		NULL,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
}

void Edit::SetSize(const mye::math::Vector2i &size)
{
	SetWindowPos(m_hEdit,
		NULL,
		NULL,
		NULL,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
}