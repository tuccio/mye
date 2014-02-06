#include <Windows.h>

#include <mye/win/Window.h>
#include <mye/win/Toolbar.h>
#include <mye/win/Utils.h>

#include <mye/d3d11/DX11ShaderManager.h>
#include <mye/d3d11/DX11BufferManager.h>

#include <mye/core/Logger.h>
#include <mye/core/GameObjectsModule.h>
#include <mye/core/MeshManager.h>
#include <mye/core/ModelManager.h>
#include <mye/core/ResourceTypeManager.h>

#include "MainWindowListener.h"
#include "Globals.h"
#include "SceneView.h"

using namespace mye::core;
using namespace mye::win;
using namespace mye::dx11;

void CompileShaders(void);

void UpdateSolution(void);

int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{

	Logger logger;

	ResourceTypeManager resourceTypeManager;
	DX11ShaderManager shaderManager(g_device);
	DX11BufferManager vertexBufferManager(g_device);
	MeshManager meshManager;
	ModelManager modelManager;

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

	g_mainWindow.Maximize();

	p.caption    = "Render window";
	p.fullscreen = false;
	p.width      = -1;
	p.height     = -1;
	p.x          = -1;
	p.y          = -1;

	g_renderWindow.SetMSAA(DX11Window::MSAA_4x);
	g_renderWindow.CreateChild(g_mainWindow, p);

	if (!g_renderWindow.Init())
	{
		ShowErrorBox("Error while initiating rendering window\nConsult logs for more details");
		exit(1);
	}

	CompileShaders();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	do
	{

		g_input.ResetDeltas();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		g_mainWindow.Update();
		g_mainWindow.Render();

	}
	while (msg.message != WM_QUIT);

	return 0;

}

