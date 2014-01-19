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

	WindowMenu &views = g_mainWindowMenu.AddSubMenu(MENUSTR_VIEWS);
	views.AddSubMenu(MENUSTR_SCENEVIEW);
	views.AddSubMenu(MENUSTR_MODELVIEW);

	g_mainWindow.AttachMenu(&g_mainWindowMenu);

}

void MainWindowListener::OnDestroy(mye::core::IWindow * window)
{
	PostQuitMessage(WM_QUIT);
}

void MainWindowListener::OnCommand(unsigned int id)
{
	g_mainWindow.DispatchCommand(id);
}

void MainWindowListener::OnMenuSelected(IDGenerator::ID id)
{

	enum {
		MENUID_FILE_NEWSOLUTION,
		MENUID_FILE_EXIT,
		MENUID_VIEWS_SCENEVIEW,
		MENUID_VIEWS_MODELVIEW,
		MENUID_N
	};

	static IDGenerator::ID ids[MENUID_N] = {
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_NEWSOLUTION].GetID(),
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_EXIT].GetID(),
		g_mainWindowMenu[MENUSTR_VIEWS][MENUSTR_SCENEVIEW].GetID(),
		g_mainWindowMenu[MENUSTR_VIEWS][MENUSTR_MODELVIEW].GetID()

	};

	int position = std::find(ids, ids + MENUID_N, id) - ids;

	switch (position)
	{

	case MENUID_FILE_NEWSOLUTION:
		// New solution stuff
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