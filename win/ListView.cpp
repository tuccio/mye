#include "ListView.h"
#include "Utils.h"
#include <mye/core/Logger.h>

#include <CommCtrl.h>

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

		auto logger = mye::core::Logger::GetSingletonPointer();
		std::string error = GetLastErrorAsString();

		if (logger)
		{
			logger->LogError("RegisterClass failed: " + error);
		}

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
		NULL,
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
		position.x(),
		position.y(),
		size.x(),
		size.y(),
		parent.GetHandle(),
		(HMENU) m_id,
		NULL,
		NULL);

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
	m_hList = NULL;
}

void ListView::SetPosition(const Vector2i &position)
{

	SetWindowPos(m_hList,
		NULL,
		position.x(),
		position.y(),
		NULL,
		NULL,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);

}

void ListView::SetSize(const Vector2i &size)
{

	SetWindowPos(m_hList,
		NULL,
		NULL,
		NULL,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);

}

bool ListView::Notify(unsigned int code, LPARAM lParam)
{

	for (Listener *listener : m_listeners)
	{
		listener->OnListViewEvent(code, lParam);
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