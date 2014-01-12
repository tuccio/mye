#include <Windows.h>

#include <mye/win/Window.h>
#include <mye/win/Toolbar.h>

#include <mye/core/Logger.h>

#include "MainWindowListener.h"
#include "Globals.h"
#include "SceneView.h"

using namespace mye::core;
using namespace mye::win;
using namespace mye::dx11;

void CompileShaders(void);

int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{

	Logger logger;
	MainWindowListener mainWindowListener;

	logger.OpenErrorLogFile("error.log");
	logger.OpenEventLogFile("event.log");

	Window::Properties p;

	p.caption    = "mye Editor";
	p.fullscreen = false;
	p.width      = -1;
	p.height     = -1;
	p.x          = -1;
	p.y          = -1;

	g_mainWindow.AddListener(&mainWindowListener);
	g_mainWindow.AddMenuListener(&mainWindowListener);

	g_mainWindow.Create(p);
	g_mainWindow.SetCaption("mye Editor");

	g_childWindow.CreateChild(g_mainWindow, p);

	g_childWindow.Show();
	g_mainWindow.Show();

	if (!g_childWindow.Init())
	{

		MessageBox(NULL,
			"Error while initiating rendering window\nConsult logs for more details",
			"Error", MB_OK);

		exit(1);

	}

	CompileShaders();


	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	do
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (g_currentView)
		{
			g_currentView->Update();
			g_currentView->Render();
		}

	}
	while (msg.message != WM_QUIT);

	return 0;

}

