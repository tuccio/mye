#include "Window.h"
#include "VirtualKeyMap.h"
#include "Utils.h"

#include <mye/core/Logger.h>

#include "CommCtrl.h"

#include "Checkbox.h"
#include "Button.h"
#include "Edit.h"
#include "ListView.h"

#include <memory>
#include <vector>
#include <deque>
#include <algorithm>

#if !defined(_T)
#if defined(_UNICODE)
#define _T(x) L ##x
#else
#define _T(x) x
#endif
#endif

#define WINDOW_CLASS_NAME _T("myeWindow")

#define WINDOW_LONG_PTR_INDEX 1

using namespace mye::math;
using namespace mye::core;
using namespace mye::win;

/* Window class registerer */

WCR::WCR(void)
{

	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = &Window::WindowProc; 
	wc.hInstance     = GetModuleHandle(nullptr);
	wc.hCursor       = LoadCursor(wc.hInstance, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wc.lpszClassName = WINDOW_CLASS_NAME;

	m_registered = (RegisterClass(&wc) ? true : false);

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

WCR::~WCR(void)
{
	if (m_registered)
	{
		UnregisterClass(WINDOW_CLASS_NAME,
			GetModuleHandle(nullptr));
	}
}

bool WCR::IsRegistered(void) const
{
	return m_registered;
}


/* Window */

Window::Window(void) :
	m_hWnd(0x0),
	m_menu(0x0),
	m_tabs(0x0)
{
}

Window::~Window(void)
{
}

bool Window::Create(const Properties &p)
{

	return _Create(0x0,
		WINDOW_CLASS_NAME,
		p.caption.CString(),
		WS_OVERLAPPEDWINDOW |
			WS_CLIPCHILDREN |
			WS_MAXIMIZE,
		(p.x < 0 ? CW_USEDEFAULT : p.x),
		(p.y < 0 ? CW_USEDEFAULT : p.y),
		(p.width < 0 ? CW_USEDEFAULT : p.width),
		(p.height < 0 ? CW_USEDEFAULT : p.height),
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		this
		);

}

bool Window::CreateChild(Window &parent, const Properties &p)
{

	return _Create(0x0,
		WINDOW_CLASS_NAME,
		p.caption.CString(),
		WS_CHILD | WS_CLIPCHILDREN,
		(p.x < 0 ? CW_USEDEFAULT : p.x),
		(p.y < 0 ? CW_USEDEFAULT : p.y),
		(p.width < 0 ? CW_USEDEFAULT : p.width),
		(p.height < 0 ? CW_USEDEFAULT : p.height),
		parent.GetHandle(),
		nullptr,
		GetModuleHandle(nullptr),
		this
		);

}

bool Window::CreateChild(HWND parent, const Properties &p)
{

	return _Create(0x0,
		WINDOW_CLASS_NAME,
		p.caption.CString(),
		WS_CHILD | WS_CLIPCHILDREN,
		(p.x < 0 ? CW_USEDEFAULT : p.x),
		(p.y < 0 ? CW_USEDEFAULT : p.y),
		(p.width < 0 ? CW_USEDEFAULT : p.width),
		(p.height < 0 ? CW_USEDEFAULT : p.height),
		parent,
		nullptr,
		GetModuleHandle(nullptr),
		this
		);

}

void Window::Destroy(void)
{
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;
}

bool Window::DispatchCommand(unsigned int id, unsigned int code)
{

	if (m_menu && m_menu->Contains(id))
	{
		NotifyMenuSelected(id);
		return true;
	}

	if (Control *control = FindControl(id))
	{
		
		switch (control->GetControlType())
		{

		case CT_BUTTON:
			static_cast<Button*>(control)->m_function();
			return true;

		case CT_CHECKBOX:
			{
				Checkbox *cb = static_cast<Checkbox*>(control);
				cb->SetCheck(!cb->IsChecked());
			}

		default:
			break;

		}

	}

	return false;

}

bool Window::Exists(void) const
{
	return (IsWindow(m_hWnd) ? true : false);
}

bool Window::IsVisible(void) const
{
	return Exists() && IsWindowVisible(m_hWnd);
}

void Window::Maximize(void)
{
	ShowWindow(m_hWnd, SW_MAXIMIZE);
}

bool Window::IsMaximized(void) const
{
	return (IsZoomed(m_hWnd) ? true : false);
}

void Window::Minimize(void)
{
	ShowWindow(m_hWnd, SW_MINIMIZE);
}

bool Window::IsMinimized(void) const
{
	return (IsIconic(m_hWnd) ? true : false);
}

void Window::Focus(void)
{
	SetFocus(m_hWnd);
	ShowWindow(m_hWnd, TRUE);
	SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void Window::Show(void)
{
	ShowWindow(m_hWnd, TRUE);
}

void Window::Hide(void)
{
	ShowWindow(m_hWnd, SW_HIDE);
}

bool Window::IsFullScreen(void) const
{
	return false;
}
void Window::SetFullScreen(void)
{
	throw;
}

void Window::SetCaption(const String &caption)
{
	SetWindowText(m_hWnd, caption.CString());
}

String Window::GetCaption(void) const
{

	int length = GetWindowTextLength(m_hWnd);

	String s(length);
	GetWindowText(m_hWnd, &s[0], length);

	return s;

}

void Window::SetSize(const mye::math::Vector2i &size)
{

	SetWindowPos(m_hWnd,
		nullptr,
		0,
		0,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER);

}

mye::math::Vector2i Window::GetSize(void) const
{

	RECT rect;

	BOOL success = GetClientRect(m_hWnd, &rect);
	assert(success);

	return mye::math::Vector2i(rect.right - rect.left,
		rect.bottom - rect.top);

}

void Window::SetPosition(const mye::math::Vector2i &position)
{

	SetWindowPos(m_hWnd,
		nullptr,
		position.x(),
		position.y(),
		0,
		0,
		SWP_NOSIZE | SWP_NOZORDER);

}

mye::math::Vector2i Window::GetPosition(void) const
{

	RECT rect;
	GetWindowRect(m_hWnd, &rect);

	return mye::math::Vector2i(rect.left, rect.top);

}

void Window::AttachMenu(WindowMenu *menu)
{
	
	m_menu = menu;

	HMENU hMenu = CreateMenu();

	_CreateMenu(menu, hMenu);

	SetMenu(m_hWnd, hMenu);
	DrawMenuBar(m_hWnd);

}

void Window::_CreateMenu(WindowMenu *menu, HMENU hMenu)
{

	if (menu->GetParent() == nullptr || true)
	{

		auto &subMenus = menu->GetSubMenus();

		for (auto &item : subMenus)
		{

			DWORD parentId = (menu->GetParent() ? menu->GetParent()->GetID() : 0);

			//InsertMenuItem(hMenu, parentId, FALSE, &info);

			if (item.second.GetSubMenus().size() > 0)
			{
				HMENU hSub = CreateMenu();
				_CreateMenu(&item.second, hSub);
				AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hSub, item.first.c_str());
			}
			else
			{
				AppendMenu(hMenu, MF_STRING, item.second.GetID(), item.first.c_str());
			}

		}

	}

}

void Window::AddMenuListener(WindowMenu::Listener *listener)
{
	m_menuListeners.push_back(listener);
}

void Window::RemoveMenuListener(WindowMenu::Listener *listener)
{
	auto it = std::find(m_menuListeners.begin(),
		m_menuListeners.end(),
		listener);

	if (it != m_menuListeners.end())
	{
		m_menuListeners.erase(it);
	}
}

void Window::ClearMenuListeners(void)
{
	m_menuListeners.clear();
}

void Window::AttachTabs(Tabs *tabs)
{
	m_tabs = tabs;
}

void Window::AddTabsListener(Tabs::Listener *listener)
{
	m_tabsListeners.push_back(listener);
}

void Window::RemoveTabsListener(Tabs::Listener *listener)
{
	auto it = std::find(m_tabsListeners.begin(),
		m_tabsListeners.end(),
		listener);

	if (it != m_tabsListeners.end())
	{
		m_tabsListeners.erase(it);
	}
}

void Window::ClearTabsListeners(void)
{
	m_tabsListeners.clear();
}

void Window::RegisterControl(Control *control)
{
	m_controls[control->GetID()] = static_cast<Control*>(control);
}

Control* Window::FindControl(IDGenerator::ID id)
{
	auto it = m_controls.find(id);
	return (it != m_controls.end() ? it->second : nullptr);
}

void Window::UnregisterControl(Control *control)
{
	m_controls.erase(
		m_controls.find(control->GetID())
	);
}
void Window::ClearControls(void)
{
	m_controls.clear();
}

void Window::NotifyMenuSelected(IDGenerator::ID id)
{
	for (auto listener : m_menuListeners)
	{
		listener->OnMenuSelected(id);
	}
}

void Window::NotifyShowTab(int index)
{
	for (auto listener : m_tabsListeners)
	{
		listener->OnTabShow(index);
	}
}

void Window::NotifyHideTab(int index)
{
	for (auto listener : m_tabsListeners)
	{
		listener->OnTabHide(index);
	}
}

HWND Window::GetHandle(void)
{
	return m_hWnd;
}

HDC Window::GetDC(void)
{
	return ::GetDC(m_hWnd);
}

bool Window::ReleaseDC(HDC hDC)
{
	return ::ReleaseDC(m_hWnd, hDC) != 0;
}

void Window::Update(void)
{
	UpdateWindow(m_hWnd);
	RedrawWindow(m_hWnd, nullptr, nullptr, RDW_UPDATENOW | RDW_ALLCHILDREN);
}

bool Window::_Create(DWORD dwExStyle,
					 LPCTSTR lpClassName,
					 LPCTSTR lpWindowName,
					 DWORD dwStyle,
					 int x,
					 int y,
					 int nWidth,
					 int nHeight,
					 HWND hWndParent,
					 HMENU hMenu,
					 HINSTANCE hInstance,
					 LPVOID lpParam)
{

	WCR *wcr = WCR::GetSingletonPointer();

	if (!wcr)
	{
		wcr = new WCR;
	}

	if (!m_hWnd && wcr->IsRegistered())
	{
		
		m_hWnd = CreateWindowEx(dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam);

		if (m_hWnd)
		{

			SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR) this);
			SetLastError(0x0);

			if (GetLastError())
			{

				auto logger = mye::core::Logger::GetSingletonPointer();

				if (logger)
				{
					logger->LogError("SetWindowLongPtr failed: " + GetLastErrorAsString());
				}

			}

		}
		else
		{

			auto logger = mye::core::Logger::GetSingletonPointer();

			if (logger)
			{
				logger->LogError("CreateWindowEx failed: " + GetLastErrorAsString());
			}

		}

	}

	if (m_hWnd)
	{
		NotifyCreate();
	}

	return m_hWnd != nullptr;

}

#define __MYE_WIN_LONGPTR_TO_WINDOW(HWND) (reinterpret_cast<Window*>(GetWindowLongPtr(HWND, GWLP_USERDATA)))
#define __MYE_WIN_WINDOW_TO_IWINDOW(WINDOW) (static_cast<mye::core::IWindow*>(WINDOW))

LRESULT CALLBACK Window::WindowProc(HWND hWnd,
									UINT uMsg,
									WPARAM wParam,
									LPARAM lParam)
{

	switch (uMsg)
	{

	case WM_ACTIVATE:

		if (wParam == WA_ACTIVE)
		{

			Window *window = __MYE_WIN_LONGPTR_TO_WINDOW(hWnd);

			if (window)
			{
				window->Update();
			}

		}
		break;

	case WM_CREATE:

		break;

	case WM_DESTROY:

		{

			Window *window = __MYE_WIN_LONGPTR_TO_WINDOW(hWnd);

			if (window)
			{
				window->NotifyDestroy();
			}

		}

		break;

	case WM_MOUSEMOVE:

	/*	{
			static HCURSOR hCursor = LoadCursor(GetModuleHandle(nullptr), IDC_ARROW);
			SetCursor(hCursor);
			ShowCursor(TRUE);
		}
*/

		break;

	case WM_COMMAND:

		{

			Window *window = __MYE_WIN_LONGPTR_TO_WINDOW(hWnd);
			unsigned int optId = LOWORD(wParam);

			if (window)
			{
				window->DispatchCommand(optId, 0x0);
			}
/*

			if (window &&
				window->m_menu)
			{

				if (window->m_menu->Contains(optId))
				{
					window->NotifyMenuSelected(optId);
				}

			}*/

		}

		break;

	case WM_SIZE:

		{

			Window *window = __MYE_WIN_LONGPTR_TO_WINDOW(hWnd);

			if (window)
			{
				window->NotifyResize(mye::math::Vector2i(LOWORD(lParam), HIWORD(lParam)));
			}

		}

		break;

	case WM_KEYDOWN:

		{

			Window *window = __MYE_WIN_LONGPTR_TO_WINDOW(hWnd);

			if (window->m_keyboard)
			{
				KeyboardVK key = VirtualKeyMap(wParam);
				
				if (key != MYE_VK_COUNT)
				{
					window->m_keyboard->Press(key);
				}
				
			}

		}

		break;

	case WM_KEYUP:

		{

			Window *window = __MYE_WIN_LONGPTR_TO_WINDOW(hWnd);

			if (window->m_keyboard)
			{
				KeyboardVK key = VirtualKeyMap(wParam);

				if (key != MYE_VK_COUNT)
				{
					window->m_keyboard->Release(key);
				}

			}

		}

		break;

	case WM_NOTIFY:

		{

			Window *window = __MYE_WIN_LONGPTR_TO_WINDOW(hWnd);
			LPNMHDR nmh = (LPNMHDR) lParam;

			if (window->m_tabs && window->m_tabs->GetHandle() == nmh->hwndFrom)
			{

				if (nmh->code == TCN_SELCHANGING)
				{
					window->NotifyHideTab(TabCtrl_GetCurSel(window->m_tabs->GetHandle()));
				}
				else if (nmh->code == TCN_SELCHANGE)
				{
					window->NotifyShowTab(TabCtrl_GetCurSel(window->m_tabs->GetHandle()));
				}
			}
			else 
			{

				Control *control = window->FindControl(nmh->idFrom);
				
				if (control)
				{
					control->Notify(nmh->code, lParam);
				}

			}

			break;

		}

	default:

		if (uMsg >= WM_USER)
		{

			Window *window = __MYE_WIN_LONGPTR_TO_WINDOW(hWnd);

			if (window)
			{
				window->NotifyUserMessage(uMsg, wParam, (long*) lParam);
			}

			break;

		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

	return 0;

}

void SendQuitOnClose::OnDestroy(IWindow *window)
{
	PostQuitMessage(WM_QUIT);
}