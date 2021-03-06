#include "ListView.h"
#include "Utils.h"
#include <mye/core/Logger.h>

#include <CommCtrl.h>

using namespace mye::core;
using namespace mye::win;
using namespace mye::math;

#pragma comment(lib, "Comctl32.lib")

ListViewWCR::ListViewWCR(void)
{

	INITCOMMONCONTROLSEX InitCtrlEx;

	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC = ICC_LISTVIEW_CLASSES;

	m_registered = (InitCommonControlsEx(&InitCtrlEx) ? true : false);

	if (!m_registered)
	{

		String error = GetLastErrorAsString();
		Logger::LogErrorOptional("WIN32 API RegisterClass", error);

		ShowErrorBox(error);

	}

}

ListViewWCR::~ListViewWCR(void)
{
}

bool ListViewWCR::IsRegistered(void) const
{
	return m_registered;
}

ListView::ListView(void) :
	Control(CT_LISTVIEW)
{
}


ListView::~ListView(void)
{
}

/* List view */

void ListView::Create(Window &parent,
			const Vector2i &position,
			const Vector2i &size)
{

	m_hList = CreateWindow( 
		WC_LISTVIEW,
		nullptr,
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
		position.x(),
		position.y(),
		size.x(),
		size.y(),
		parent.GetHandle(),
		(HMENU) m_id,
		nullptr,
		nullptr);

	SendMessage(m_hList, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), (LPARAM) TRUE);
	SendMessage(m_hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	if (!m_hList)
	{
		ShowErrorBox();
	}

}

void ListView::Destroy(void)
{
	DeleteObject(m_hList);
	m_hList = nullptr;
}

void ListView::SetPosition(const Vector2i &position)
{

	SetWindowPos(m_hList,
		nullptr,
		position.x(),
		position.y(),
		0,
		0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);

}

void ListView::SetSize(const Vector2i &size)
{

	SetWindowPos(m_hList,
		nullptr,
		0,
		0,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);

}

bool ListView::Notify(unsigned int code, LPARAM lParam)
{

	for (Listener *listener : m_listeners)
	{
		listener->OnListViewEvent(this, code, lParam);
	}

	return true;

}

void ListView::AddListener(ListView::Listener *listener)
{
	m_listeners.push_back(listener);
}
void ListView::RemoveListener(ListView::Listener *listener)
{
	m_listeners.erase(std::find(m_listeners.begin(), m_listeners.end(), listener));
}

void ListView::ClearListeners(void)
{
	m_listeners.clear();
}

HWND ListView::GetHandle(void)
{
	return m_hList;
}