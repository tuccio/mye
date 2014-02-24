#include "Tabs.h"
#include "Utils.h"
#include "Window.h"

#include <mye/core/Logger.h>

#include <CommCtrl.h>

using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

#pragma comment(lib, "Comctl32.lib")

TabsWCR::TabsWCR(void)
{

	INITCOMMONCONTROLSEX InitCtrlEx;

	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC = ICC_TAB_CLASSES;

	m_registered = (InitCommonControlsEx(&InitCtrlEx) ? true : false);

	if (!m_registered)
	{

		auto logger = mye::core::Logger::GetSingletonPointer();
		String error = GetLastErrorAsString();

		if (logger)
		{
			logger->LogError("RegisterClass failed: " + error);
		}

		ShowErrorBox(error);

	}

}

TabsWCR::~TabsWCR(void)
{
}

bool TabsWCR::IsRegistered(void) const
{
	return m_registered;
}

/* Tabs */

Tabs::Tabs(Window &window) :
	m_parent(window)
{
}

Tabs::~Tabs(void)
{
}

void Tabs::Create(void)
{

	if (!TabsWCR::GetSingletonPointer())
	{
		new TabsWCR;
	}

	Vector2i parentSize = m_parent.GetSize();
	
	m_hTabs = CreateWindowEx(
		0,
		WC_TABCONTROL,
		nullptr,
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | TCS_MULTILINE,
		0,
		0,
		parentSize.x(),
		parentSize.y(),
		m_parent.GetHandle(),
		(HMENU) IDGenerator::GetSingleton().Generate(),
		nullptr,
		nullptr);

	m_parent.AttachTabs(this);

	SendMessage(m_hTabs, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), (LPARAM) TRUE);

	if (!m_hTabs)
	{
		ShowErrorBox();
		return;
	}

}

void Tabs::Destroy(void)
{
	m_parent.AttachTabs(nullptr);
	CloseWindow(m_hTabs);
	m_hTabs = nullptr;
}

void Tabs::Show(void)
{
	ShowWindow(m_hTabs, SW_SHOW);
}

void Tabs::Hide(void)
{
	ShowWindow(m_hTabs, SW_HIDE);
}

void Tabs::SetPosition(const Vector2i &position)
{

	SetWindowPos(m_hTabs,
		nullptr,
		position.x(),
		position.y(),
		nullptr,
		nullptr,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);

}

void Tabs::SetSize(const Vector2i &size)
{
	
	SetWindowPos(m_hTabs,
		nullptr,
		nullptr,
		nullptr,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);

}

void Tabs::SelectTab(int index)
{
	TabCtrl_SetCurSel(m_hTabs, index);
}

int Tabs::GetSelectedTab(void)
{
	return TabCtrl_GetCurSel(m_hTabs);
}

HWND Tabs::GetHandle(void)
{
	return m_hTabs;
}

void Tabs::AddTab(int index, const std::string &label)
{
	
	TCITEM item;

	item.mask = TCIF_TEXT;
	item.pszText = (LPSTR) label.c_str();

	SendMessage(m_hTabs, TCM_INSERTITEM, index, (LPARAM) &item);

}

void Tabs::Update(void)
{
	UpdateWindow(m_hTabs);
}

void Tabs::Listener::OnTabHide(int index)
{

}

void Tabs::Listener::OnTabShow(int index)
{

}