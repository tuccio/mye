#include "RenderWindow.h"
#include "Util.h"

#include <mye/core/Logger.h>

#include <memory>

#if !defined(_T)
#if defined(_UNICODE)
#define _T(x) L ##x
#else
#define _T(x) x
#endif
#endif

#define WINDOW_CLASS_NAME _T("myeWindow")

using namespace mye::win;

const RenderWindow::Properties RenderWindow::_defaultWindowProperties =
{
	WINDOW_CLASS_NAME,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	false
};

/* Window class registerer */

RenderWindow::WindowClassRegisterer RenderWindow::_wcr;

RenderWindow::WindowClassRegisterer::WindowClassRegisterer(void)
{
	Register();
}

bool RenderWindow::WindowClassRegisterer::IsRegistered(void) const
{
	return _registered;
}

void RenderWindow::WindowClassRegisterer::Register(void)
{

	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));

	//wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.style = WS_OVERLAPPEDWINDOW;
	wc.lpfnWndProc = &WindowProc; 
	wc.hInstance = GetModuleHandle(NULL);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = WINDOW_CLASS_NAME;

	_registered = (RegisterClass(&wc) ? true : false);

	if (!_registered)
	{

		auto logger = mye::core::Logger::GetSingletonPointer();

		if (logger)
		{
			logger->LogError(GetLastErrorAsString());
		}

	}

}

/* Window */

RenderWindow::RenderWindow(void) :
	_hWnd(0x0),
	_hDC(0x0)
{
}


RenderWindow::~RenderWindow(void)
{
}

bool RenderWindow::Create(void)
{
	return Create(_defaultWindowProperties);
}

bool RenderWindow::Create(const Properties &p)
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

void RenderWindow::Destroy(void)
{
	DestroyWindow(_hWnd);
	_hWnd = NULL;
}

bool RenderWindow::Exists(void) const
{
	return IsWindow(_hWnd);
}

bool RenderWindow::IsVisible(void) const
{
	return Exists() && IsWindowVisible(_hWnd);
}

bool RenderWindow::IsMinimized(void) const
{
	return (IsIconic(_hWnd) ? true : false);
}

void RenderWindow::Show(void)
{
	ShowWindow(_hWnd, TRUE);
}

void RenderWindow::Hide(void)
{
	ShowWindow(_hWnd, FALSE);
}

bool RenderWindow::IsFullScreen(void) const
{
	throw;
	return false;
}
void RenderWindow::SetFullScreen(void)
{
	throw;
}

void RenderWindow::SetCaption(const std::string &caption)
{
	SetWindowText(_hWnd, caption.c_str());
}

std::string RenderWindow::GetCaption(void) const
{
	
	int length = GetWindowTextLength(_hWnd);

	std::vector<char> v(length + 1);
	GetWindowText(_hWnd, &v[0], length);

	return std::string(&v[0]);

}

void RenderWindow::SetSize(const Eigen::Vector2i &size)
{

	SetWindowPos(_hWnd,
		NULL,
		NULL,
		NULL,
		size.x(),
		size.y(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);

}

Eigen::Vector2i RenderWindow::GetSize(void) const
{

	RECT rect;
	GetClientRect(_hWnd, &rect);

	return Eigen::Vector2i(rect.right - rect.left,
		rect.bottom - rect.top);

}

void RenderWindow::SetPosition(const Eigen::Vector2i &position)
{

	SetWindowPos(_hWnd,
		NULL,
		position.x(),
		position.y(),
		NULL,
		NULL,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);

}

Eigen::Vector2i RenderWindow::GetPosition(void) const
{

	RECT rect;
	GetWindowRect(_hWnd, &rect);

	return Eigen::Vector2i(rect.left, rect.top);

}

LRESULT CALLBACK RenderWindow::WindowProc(HWND hWnd,
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