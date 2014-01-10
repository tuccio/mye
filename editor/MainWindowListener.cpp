#include "MainWindowListener.h"
#include "Globals.h"

using namespace mye::win;

#define MENUSTR_FILE "File"
#define MENUSTR_NEWSOLUTION "New Solution"
#define MENUSTR_OPENSOLUTION "Open Solution"
#define MENUSTR_SAVESOLUTION "Save Solution"
#define MENUSTR_SAVESOLUTIONAS "Save Solution As..."
#define MENUSTR_EXIT "Exit"
#define MENUSTR_VIEWS "Views"
#define MENUSTR_SCENEVIEW "Scene View"

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

	g_mainWindow.AttachMenu(&g_mainWindowMenu);

}

void MainWindowListener::OnDestroy(mye::core::IWindow * window)
{
	PostQuitMessage(WM_QUIT);
}

void MainWindowListener::OnResize(mye::core::IWindow *window,
								  const Eigen::Vector2i &size)
{

	if (g_currentView)
	{
		g_currentView->Resize(size);
	}

}

void MainWindowListener::OnMenuSelected(IDGenerator::ID id)
{

	enum {
		MENUID_FILE_NEWSOLUTION,
		MENUID_FILE_EXIT,
		MENUID_VIEWS_SCENEVIEW,
		MENUID_N
	};

	static IDGenerator::ID ids[MENUID_N] = {
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_NEWSOLUTION].GetID(),
		g_mainWindowMenu[MENUSTR_FILE][MENUSTR_EXIT].GetID(),
		g_mainWindowMenu[MENUSTR_VIEWS][MENUSTR_SCENEVIEW].GetID()
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
		g_currentView = &g_sceneView;
		g_currentView->Resize(g_mainWindow.GetSize());
		break;

	}

}