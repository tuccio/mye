#include "ToolBar.h"
#include "Util.h"

#include <mye/core/Logger.h>

#define TOOLBAR_CLASS_NAME "myeToolbar"

using namespace mye::win;

ToolbarWCR::ToolbarWCR(void)
{

	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = &Toolbar::WindowProc; 
	wc.hInstance = GetModuleHandle(NULL);
	wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wc.lpszClassName = TOOLBAR_CLASS_NAME;

	m_registered = (RegisterClass(&wc) ? true : false);

	if (!m_registered)
	{

		auto logger = mye::core::Logger::GetSingletonPointer();

		if (logger)
		{
			logger->LogError("Render window registering error: " + GetLastErrorAsString());
		}

	}

}

ToolbarWCR::~ToolbarWCR(void)
{
	if (m_registered)
	{
		UnregisterClass(TOOLBAR_CLASS_NAME,
			GetModuleHandle(NULL));
	}
}

bool ToolbarWCR::IsRegistered(void) const
{
	return m_registered;
}

/* Toolbar */

Toolbar::Toolbar(Window &window,
				 bool vertical)
{

	m_vertical = vertical;

}


Toolbar::~Toolbar(void)
{
}

LRESULT CALLBACK Toolbar::WindowProc(HWND hWnd,
									 UINT uMsg,
									 WPARAM wParam,
									 LPARAM lParam)
{

	switch (uMsg)
	{

	case WM_CREATE:

		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

	return 0;

}