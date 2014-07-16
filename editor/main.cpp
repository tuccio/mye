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

	if (!g_dx11graphics.GetDevice()->Create())
	{
		ShowErrorBox("Cannot create DX11 device");
		exit(1);
	}

	MainWindowListener mainWindowListener;

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

	if (!g_renderWindow.CreateChild(g_mainWindow, p))
	{
		ShowErrorBox("Error while creating window");
		exit(1);
	}

	g_dx11graphics.SetWindow(&g_renderWindow);

	DX11ShaderManager shaderManager(*g_dx11graphics.GetDevice());
	DX11BufferManager vertexBufferManager(*g_dx11graphics.GetDevice());
	MeshManager meshManager;
	ModelManager modelManager;

	DX11SwapChainConfiguration swapChainConf;

	swapChainConf.device     = g_dx11graphics.GetDevice();
	swapChainConf.format     = DataFormat::RGBA32;

	swapChainConf.msaa       = MSAA::MSAA_OFF;
	swapChainConf.window     = &g_renderWindow;
	swapChainConf.fullscreen = false;

	swapChainConf.width      = 0;
	swapChainConf.height     = 0;

	swapChainConf.refresh    = mye::math::Rational<unsigned int>(1, 60);

	g_swapChain = DX11SwapChain(swapChainConf);

	DX11DepthBufferConfiguration depthBufferConf;

	depthBufferConf.device = g_dx11graphics.GetDevice();
	depthBufferConf.height = 0;
	depthBufferConf.width  = 0;

	g_depthBuffer = DX11DepthBuffer(depthBufferConf);

	if (!g_dx11graphics.Init())
	{
		ShowErrorBox("Cannot initialize DX11");
		exit(1);
	}

	if (!g_swapChain.Create() ||
		!g_depthBuffer.Create())
	{
		ShowErrorBox("Error while initiating swapchain");
		exit(1);
	}

	

	g_mainWindow.SetSplitScreen(1, 1);
	g_mainWindow.SetSplitView(0, 0, &g_sceneView);
	g_mainWindow.ResizeViews();

	mainWindowListener.OnResize(&g_mainWindow, g_mainWindow.GetSize());
	
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

