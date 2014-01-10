#include "Window.h"
#include "Util.h"

#include <mye/core/Logger.h>

#include <memory>
#include <vector>
#include <deque>>
#if !defined(_T)
#if defined(_UNICODE)
#define _T(x) L ##x
#else
#define _T(x) x
#endif
#endif

#define WINDOW_CLASS_NAME _T("myeWindow")

#define WINDOW_LONG_PTR_INDEX 1

using namespace mye::win;

/* Window class registerer */

WCR::WCR(void)
{

	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = &Window::WindowProc; 
	wc.hInstance     = GetModuleHandle(NULL);
	wc.hCursor       = LoadCursor(wc.hInstance, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wc.lpszClassName = WINDOW_CLASS_NAME;

	m_registered = (RegisterClass(&wc) ? true : false);

	if (!m_registered)
	{

		auto logger = mye::core::Logger::GetSingletonPointer();

		if (logger)
		{
			logger->LogError("RegisterClass failed: " + GetLastErrorAsString());
		}

	}

}

WCR::~WCR(void)
{
	if (m_registered)
	{
		UnregisterClass(WINDOW_CLASS_NAME,
			GetModuleHandle(NULL));
	}
}

bool WCR::IsRegistered(void) const
{
	return m_registered;
}


/* Window */

Window::Window(void) :
	m_hWnd(0x0),
	m_menu(0x0)
{
}

Window::~Window(void)
{
}

bool Window::Create(const Properties &p)
{

	return _Create(0x0,
		WINDOW_CLASS_NAME,
		p.caption.c_str(),
		WS_OVERLAPPEDWINDOW |
			WS_CLIPCHILDREN |
			WS_MAXIMIZE,
		(p.x < 0 ? CW_USEDEFAULT : p.x),
		(p.y < 0 ? CW_USEDEFAULT : p.y),
		(p.width < 0 ? CW_USEDEFAULT : p.width),
		(p.height < 0 ? CW_USEDEFAULT : p.height),
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this
		);

}

bool Window::CreateChild(Window &parent, const Properties &p)
{

	return _Create(0x0,
		WINDOW_CLASS_NAME,
		p.caption.c_str(),
		WS_CHILD,
		(p.x < 0 ? CW_USEDEFAULT : p.x),
		(p.y < 0 ? CW_USEDEFAULT : p.y),
		(p.width < 0 ? CW_USEDEFAULT : p.width),
		(p.height < 0 ? CW_USEDEFAULT : p.height),
		parent.GetHandle(),
		NULL,
		GetModuleHandle(NULL),
		this
		);

}

void Window::Destroy(void)
{
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
}

bool Window::Exists(void) const
{
	return IsWindow(m_hWnd);
}

bool Window::IsVisible(void) const
{
	return Exists() && IsWindowVisible(m_hWnd);
}

bool Window::IsMinimized(void) const
{
	return (IsIconic(m_hWnd) ? true : false);
}

void Window::Show(void)
{
	ShowWindow(m_hWnd, TRUE);
}

void Window::Hide(void)
{
	ShowWindow(m_hWnd, FALSE);
}

bool Window::IsFullScreen(void) const
{
	throw;
	return false;
}
void Window::SetFullScreen(void)
{
	throw;
}

void Window::SetCaption(const std::string &caption)
{
	SetWindowText(m_hWnd, caption.c_str());
}

std::string Window::GetCaption(void) const
{

	int length = GetWindowTextLength(m_hWnd);

	std::vector<char> v(length + 1);
	GetWindowText(m_hWnd, &v[0], length);

	return std::string(&v[0]);

}

void Window::SetSize(const Eigen::Vector2i &size)
{

	SetWindowPos(m_hWnd,
		NULL,
		NULL,
		NULL,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);

}

Eigen::Vector2i Window::GetSize(void) const
{

	RECT rect;

	BOOL success = GetClientRect(m_hWnd, &rect);
	assert(success);

	return Eigen::Vector2i(rect.right - rect.left,
		rect.bottom - rect.top);

}

void Window::SetPosition(const Eigen::Vector2i &position)
{

	SetWindowPos(m_hWnd,
		NULL,
		position.x(),
		position.y(),
		NULL,
		NULL,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);

}

Eigen::Vector2i Window::GetPosition(void) const
{

	RECT rect;
	GetWindowRect(m_hWnd, &rect);

	return Eigen::Vector2i(rect.left, rect.top);

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

	if (menu->GetParent() == NULL || true)
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

void Window::NotifyMenuSelected(IDGenerator::ID id)
{
	for (auto listener : m_menuListeners)
	{
		listener->OnMenuSelected(id);
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

	return m_hWnd != NULL;

}

LRESULT CALLBACK Window::WindowProc(HWND hWnd,
									UINT uMsg,
									WPARAM wParam,
									LPARAM lParam)
{

	switch (uMsg)
	{

	case WM_CREATE:

		break;
/*

		((Window*) lParam)->NotifyCreate();*/
		break;

	case WM_DESTROY:

		{

			Window *window = (Window*) GetWindowLongPtr(hWnd, GWLP_USERDATA);

			if (window)
			{
				window->NotifyDestroy();
			}

		}

		break;

	case WM_MOUSEMOVE:

	/*	{
			static HCURSOR hCursor = LoadCursor(GetModuleHandle(NULL), IDC_ARROW);
			SetCursor(hCursor);
			ShowCursor(TRUE);
		}
*/

		break;

	case WM_COMMAND:

		{

			Window *window = (Window*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
			unsigned int optId = LOWORD(wParam);

			if (window &&
				window->m_menu &&
				window->m_menu->Contains(optId))
			{
				window->NotifyMenuSelected(optId);
			}

		}

		break;

	case WM_SIZE:

		{

			Window *window = (Window*) GetWindowLongPtr(hWnd, GWLP_USERDATA);

			if (window)
			{
				window->NotifyResize(Eigen::Vector2i(LOWORD(lParam), HIWORD(lParam)));
			}

		}

		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

	return 0;

}