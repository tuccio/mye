#include "Window.h"

#include <memory>

#if !defined(_T)
#if defined(_UNICODE)
#define _T(x) L ##x
#else
#define _T(x) x
#endif
#endif

#define WINDOW_CLASS_NAME _T("myeD3DWindow")

using namespace mye::win;

const Window::Properties Window::_defaultWindowProperties =
{
	WINDOW_CLASS_NAME,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	false
};

/* Window class registerer */

Window::WindowClassRegisterer Window::_wcr;

Window::WindowClassRegisterer::WindowClassRegisterer(void)
{
	Register();
}

bool Window::WindowClassRegisterer::IsRegistered(void) const
{
	return _registered;
}

void Window::WindowClassRegisterer::Register(void)
{

	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = &WindowProc; 
	wc.hInstance = GetModuleHandle(NULL);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = WINDOW_CLASS_NAME;

	_registered = (RegisterClass(&wc) ? true : false);

}

/* Window */

Window::Window(void) :
	_hWnd(0x0),
	_hDC(0x0)
{
}


Window::~Window(void)
{
}

bool Window::Create(void)
{
	return Create(_defaultWindowProperties);
}

bool Window::Create(const Properties &p)
{

	if (!_hWnd && _wcr.IsRegistered())
	{

		_hWnd = CreateWindow(WINDOW_CLASS_NAME,
			p.name.c_str(),
			WS_OVERLAPPEDWINDOW,
			(p.x < 0 ? CW_USEDEFAULT : p.x),
			(p.y < 0 ? CW_USEDEFAULT : p.y),
			p.width,
			p.height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			this);

	}

	return _hWnd != NULL;

}

void Window::Destroy(void)
{
	DestroyWindow(_hWnd);
	_hWnd = NULL;
}

bool Window::Exists(void) const
{
	return IsWindow(_hWnd);
}

bool Window::IsVisible(void) const
{
	return Exists() && IsWindowVisible(_hWnd);
}

bool Window::IsMinimized(void) const
{
	return (IsIconic(_hWnd) ? true : false);
}

void Window::Show(void)
{
	ShowWindow(_hWnd, TRUE);
}

void Window::Hide(void)
{
	ShowWindow(_hWnd, FALSE);
}

void Window::SetCaption(const std::string &caption)
{
	SetWindowText(_hWnd, caption.c_str());
}

std::string Window::GetCaption(void) const
{
	
	int length = GetWindowTextLength(_hWnd);

	std::vector<char> v(length + 1);
	GetWindowText(_hWnd, &v[0], length);

	return std::string(&v[0]);

}

void Window::SetSize(const Eigen::Vector2i &size)
{

	SetWindowPos(_hWnd,
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
	GetClientRect(_hWnd, &rect);

	return Eigen::Vector2i(rect.right - rect.left,
		rect.bottom - rect.top);

}

void Window::SetPosition(const Eigen::Vector2i &position)
{

	SetWindowPos(_hWnd,
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
	GetWindowRect(_hWnd, &rect);

	return Eigen::Vector2i(rect.left, rect.top);

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

	case WM_DESTROY:

		PostQuitMessage(WM_QUIT);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

	return 0;

}