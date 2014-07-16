#include "MainWindowListener.h"
#include "Globals.h"

using namespace mye::core;
using namespace mye::win;

#define MENUSTR_FILE "File"
#define MENUSTR_NEWSOLUTION "New Solution"
#define MENUSTR_OPENSOLUTION "Open Solution"
#define MENUSTR_SAVESOLUTION "Save Solution"
#define MENUSTR_SAVESOLUTIONAS "Save Solution As..."
#define MENUSTR_EXIT "Exit"
#define MENUSTR_VIEWS "Views"
#define MENUSTR_SCENEVIEW "Scene View"
#define MENUSTR_MODELVIEW "Model View"

MainWindowListener::MainWindowListener(void)
{
}


MainWindowListener::~MainWindowListener(void)
{
}

void MainWindowListener::OnCreate(mye::core::IWindow * window)
{

	WindowMenu &file = g_mainWindowMenu.AddSubMenu(MENUSTR_FILE);
	file.AddSubMenu(MENUSTR_NEWSOLUTION);
	file.AddSubMenu(MENUSTR_OPENSOLUTION);
	file.AddSubMenu(MENUSTR_SAVESOLUTION);
	file.AddSubMenu(MENUSTR_SAVESOLUTIONAS);
	file.AddSubMenu(MENUSTR_EXIT);

// 	WindowMenu &views = g_mainWindowMenu.AddSubMenu(MENUSTR_VIEWS);
// 	views.AddSubMenu(MENUSTR_SCENEVIEW);
// 	views.AddSubMenu(MENUSTR_MODELVIEW);

	g_mainWindow.AttachMenu(&g_mainWindowMenu);

}

void MainWindowListener::OnDestroy(mye::core::IWindow * window)
{
	PostQuitMessage(WM_QUIT);
}

void MainWindowListener::OnCommand(unsigned int id, unsigned int code)
{
	g_mainWindow.DispatchCommand(id, code);
}

void MainWindowListener::OnMenuSelected(IDGenerator::ID id)
{

	enum {
		MENUID_FILE_NEWSOLUTION,
		MEUNID_FILE_OPENSOLUTION,
		MENUID_FILE_SAVESOLUTION,
		MENUID_FILE_SAVESOLUTIONAS,
		MENUID_FILE_EXIT,
		MENUID_VIEWS_SCENEVIEW,
		MENUID_VIEWS_MODELVIEW,
		MENUID_N
	};

	static IDGenerator::ID ids[MENUID_N] = {
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_NEWSOLUTION].GetID(),
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_OPENSOLUTION].GetID(),
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_SAVESOLUTION].GetID(),
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_SAVESOLUTIONAS].GetID(),
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_EXIT].GetID(),
// 		g_mainWindowMenu[MENUSTR_VIEWS][MENUSTR_SCENEVIEW].GetID(),
// 		g_mainWindowMenu[MENUSTR_VIEWS][MENUSTR_MODELVIEW].GetID()

	};

	int position = std::find(ids, ids + MENUID_N, id) - ids;

	switch (position)
	{

	case MENUID_FILE_NEWSOLUTION:
		// New solution stuff
		break;

	case MEUNID_FILE_OPENSOLUTION:

		{

			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			char buffer[256] = { 0 };

			ofn.lStructSize     = sizeof(OPENFILENAME);
			ofn.hwndOwner       = g_mainWindow.GetHandle();
			ofn.lpstrFile       = buffer;
			ofn.nMaxFile        = sizeof(buffer);
			ofn.lpstrFilter     = "mye Scene\0*.mys\0";
			ofn.nFilterIndex    = 1;
			ofn.lpstrFileTitle  = nullptr;
			ofn.nMaxFileTitle   = 0;
			ofn.lpstrInitialDir = nullptr;
			ofn.Flags           = 0;

			if (GetOpenFileName(&ofn))
			{
				g_currentSolution = ofn.lpstrFile;

				std::list<GameObject*> allocatedObjects;
				g_game.ImportScene(g_currentSolution, &allocatedObjects);

				for (GameObject *gameObject : allocatedObjects)
				{
					g_sceneView._AddGameObject(gameObject->GetHandle());
				}

			}

		}

		break;

	case MENUID_FILE_SAVESOLUTION:
		break;

	case MENUID_FILE_SAVESOLUTIONAS:

		{

			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			char buffer[256] = { 0 };

			ofn.lStructSize     = sizeof(OPENFILENAME);
			ofn.hwndOwner       = g_mainWindow.GetHandle();
			ofn.lpstrFile       = buffer;
			ofn.nMaxFile        = sizeof(buffer);
			ofn.lpstrFilter     = "mye Scene\0*.mys\0";
			ofn.nFilterIndex    = 1;
			ofn.lpstrFileTitle  = nullptr;
			ofn.nMaxFileTitle   = 0;
			ofn.lpstrInitialDir = nullptr;
			ofn.Flags           = 0;

			if (GetOpenFileName(&ofn))
			{
				g_currentSolution = ofn.lpstrFile;
				g_game.ExportScene(g_currentSolution);
			}

		}

		break;

	case MENUID_FILE_EXIT:
		PostQuitMessage(WM_QUIT);
		break;

	case MENUID_VIEWS_SCENEVIEW:

		g_mainWindow.SetSplitScreen(1, 1);
		g_mainWindow.SetSplitView(0, 0, &g_sceneView);
		g_mainWindow.ResizeViews();
		break;

	case MENUID_VIEWS_MODELVIEW:

		g_mainWindow.SetSplitScreen(1, 1);
		g_mainWindow.SetSplitView(0, 0, &g_modelView);
		g_mainWindow.ResizeViews();
		break;

	}

}

void MainWindowListener::OnResize(mye::core::IWindow *window, const mye::math::Vector2i &size)
{

	if (g_renderWindow.Exists())
	{

		Camera *camera = g_scene.GetCamera();

		auto renderWindowSize = g_renderWindow.GetSize();

		if (camera)
		{
			camera->SetClipAspectRatio((float)renderWindowSize.x() / renderWindowSize.y());
		}

		if (g_swapChain.Exists())
		{
			g_swapChain.Resize(renderWindowSize.x(), renderWindowSize.y());
		}

		if (g_swapChain.Exists())
		{

			D3D11_VIEWPORT viewPort;

			viewPort.MinDepth = 0.0f;
			viewPort.MaxDepth = 1.0f;
			viewPort.TopLeftX = 0.0f;
			viewPort.TopLeftY = 0.0f;
			viewPort.Width = (float)renderWindowSize.x();
			viewPort.Height = (float)renderWindowSize.y();

			g_dx11graphics.GetDevice()->GetImmediateContext()->RSSetViewports(1, &viewPort);

		}

	}

}