#include <Windows.h>

#include <mye/win/Window.h>
#include <mye/win/Toolbar.h>

#include <mye/core/Logger.h>

#include "MainWindowListener.h"
#include "Globals.h"

using namespace mye::core;
using namespace mye::win;

int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{

	Logger logger;

	logger.OpenErrorLogFile("error.log");
	logger.OpenEventLogFile("event.log");

	Window::Properties p;

	p.caption    = "mye Editor";
	p.fullscreen = false;
	p.width      = -1;
	p.height     = -1;
	p.x          = -1;
	p.y          = -1;

	g_mainWindow.AddListener(new MainWindowListener);

	g_mainWindow.Create(p);
	g_mainWindow.SetCaption("mye Editor");

	g_renderWindow.CreateChild(g_mainWindow, p);
	g_renderWindow.Show();

	g_mainWindow.Show();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	do
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
	while (msg.message != WM_QUIT);

	return 0;

}