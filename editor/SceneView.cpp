#include "SceneView.h"
#include "Globals.h"

#include <mye/win/Button.h>
#include <mye/win/ListView.h>
#include <mye/win/Utils.h>

#include <cstdlib>

#undef max

#define TAB_GAME_OBJECTS 0
#define TAB_PROPERTIES 1
#define TAB_TRANSFORM 2
#define TAB_RENDERER 3
#define TAB_BEHAVIOUR 4

using namespace mye::dx11;
using namespace mye::win;
using namespace mye::math;
using namespace mye::core;

SceneView::SceneView(DX11Device &device) :
	m_window(device),
	m_device(device),
	m_bgColor(0.12f, 0.12f, 0.12f, 1.0f),
	m_initialized(false),
	m_tabs(m_tabsWindow),
	m_selected(NULL),
	m_activeTab(-1)
{
}


SceneView::~SceneView(void)
{

	for (auto it : m_controls)
	{
		it.second->Destroy();
		delete it.second;
	}

}

void SceneView::Activate(void)
{

	if (!m_initialized)
	{

		Window::Properties p;

		p.caption    = "Scene View";
		p.fullscreen = false;
		p.width      = -1;
		p.height     = -1;
		p.x          = -1;
		p.y          = -1;

		m_window.CreateChild(g_mainWindow, p);
		
		if (!m_window.Init())
		{
			ShowErrorBox("Error while initiating rendering window\nConsult logs for more details");
			exit(1);
		}

		m_tabsWindow.CreateChild(g_mainWindow, p);
		m_tabs.Create();

		m_tabsWindow.AddTabsListener(this);

		m_tabs.AddTab(TAB_GAME_OBJECTS, "Game Objects");
		m_tabs.AddTab(TAB_PROPERTIES, "Properties");
		m_tabs.AddTab(TAB_TRANSFORM, "Transform");
		m_tabs.AddTab(TAB_RENDERER, "Renderer");
		m_tabs.AddTab(TAB_BEHAVIOUR, "Behaviour");

		_CreateGameObjectsTab();

		m_tabs.SelectTab(-1);

		m_initialized = true;

	}

	g_mainWindow.ResizeViews();

	m_tabs.Show();
	m_tabsWindow.Show();
	m_window.Show();

}

void SceneView::Deactivate(void)
{
	m_window.Hide();
	m_tabsWindow.Hide();
	m_tabs.Hide();
}

void SceneView::SetPosition(const mye::math::Vector2i &position)
{
	m_window.SetPosition(position);
}

void SceneView::SetSize(const mye::math::Vector2i &size)
{

	const int rightPanelWidth = 280;
	const int tabPadding = 8;
	const int tabHeight = 50;

	View::SetSize(size);

	Vector2i rightPanel(rightPanelWidth, size.y());
	Vector2i leftPanel(size.x() - rightPanelWidth, size.y());

	m_window.SetSize(leftPanel);

 	m_tabsWindow.SetPosition(Vector2i(leftPanel.x(), 0));
 	m_tabsWindow.SetSize(rightPanel);

	Vector2i tabSize = Vector2i(rightPanel.x() - 2 * tabPadding, rightPanel.y() - tabHeight - tabPadding);
	Vector2i tabPosition = Vector2i(tabPadding, tabHeight);

	m_gameObjectsTab.SetSize(tabSize);
	m_gameObjectsTab.SetPosition(tabPosition);

	auto goList = m_controls.find("GOTGameObjectsList");

	if (goList != m_controls.end())
	{
		goList->second->SetSize(Vector2i(rightPanel.x() - 2 * tabPadding, rightPanel.y() - 24 - tabPadding));
	}

	m_tabs.SetSize(rightPanel);

	m_tabsWindow.Update();

}

void SceneView::SetBackgroundColor(const mye::core::ColorRGBA &rgba)
{
	m_bgColor = rgba;
}

void SceneView::OnTabShow(int index)
{

	switch (index)
	{

	case TAB_GAME_OBJECTS:
		m_gameObjectsTab.Show();
		UpdateWindow(m_tabs.GetHandle());
		break;

	case TAB_PROPERTIES:
		break;

	case TAB_TRANSFORM:
		break;

	case TAB_RENDERER:
		break;

	case TAB_BEHAVIOUR:
		break;

	default:
		break;

	}

	m_activeTab = index;

}

void SceneView::OnTabHide(int index)
{
	switch (index)
	{

	case TAB_GAME_OBJECTS:
		m_gameObjectsTab.Hide();
		break;

	case TAB_PROPERTIES:
		break;

	case TAB_TRANSFORM:
		break;

	case TAB_RENDERER:
		break;

	case TAB_BEHAVIOUR:
		break;

	default:
		break;

	}
}

void SceneView::Update(void)
{

	switch (m_activeTab)
	{

	case TAB_GAME_OBJECTS:
		break;

	default:
		break;

	}

}

void SceneView::Render(void)
{

	m_device.GetImmediateContext()->ClearRenderTargetView(
		m_window.GetRenderTargetView(),
		m_bgColor.Data());

	m_device.GetImmediateContext()->ClearDepthStencilView(
		m_window.GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1,
		0);

	m_window.GetSwapChain()->Present(1, 0);

}

/* Tabs */

void SceneView::_CreateGameObjectsTab(void)
{

	m_gameObjectsTab.CreateChild(m_tabs.GetHandle());

	ListView *gameObjectsList = new ListView;
	m_controls["GOTGameObjectsList"] = gameObjectsList;

	gameObjectsList->Create(
		m_gameObjectsTab,
		Vector2i(0, 32));

	Button *createButton = new Button;
	m_controls["GOTCreateButton"] = createButton;

	createButton->Create(
		m_gameObjectsTab,
		"Create",
		[gameObjectsList] (void) -> void
		{

			GameObjectHandle hObj = g_gameObjectsModule.Create();

			LV_ITEM item;
			ZeroMemory(&item, sizeof(LV_ITEM));
			item.cchTextMax = 64;
			item.mask = LVIF_TEXT | LVIF_PARAM;
			item.pszText = "<Anon>";
			item.lParam = (LPARAM) g_gameObjectsModule.Get(hObj);

			ListView_InsertItem(gameObjectsList->GetHandle(), &item);
			//ListView_SetItemText(gameObjectsList->GetHandle(), 0, 1, TEXT("<None>"));

		},
		Vector2i(0, 0),
		Vector2i(120, 28));

	Button *deleteButton = new Button;
		m_controls["GOTDeleteButton"] = deleteButton;

	deleteButton->Create(
			m_gameObjectsTab,
			"Delete",
			[gameObjectsList] (void) -> void
		{

			int selected = ListView_GetNextItem(gameObjectsList->GetHandle(), -1, LVNI_SELECTED);

			if (selected >= 0)
			{

				LV_ITEM item;
				ZeroMemory(&item, sizeof(LV_ITEM));
				item.mask = LVIF_PARAM;
				item.iItem = selected;
				
				BOOL itemSelected = ListView_GetItem(gameObjectsList->GetHandle(), &item);

				if (itemSelected && item.lParam)
				{
					GameObjectHandle hObj = ((GameObject*) (item.lParam))->GetHandle();
					g_gameObjectsModule.Destroy(hObj);
					ListView_DeleteItem(gameObjectsList->GetHandle(), selected);
				}

			}

		},
			Vector2i(128, 0),
			Vector2i(120, 28));

	LV_COLUMN nameCol;
	nameCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	nameCol.pszText = "Name";
	nameCol.cx = 280;

	ListView_InsertColumn(gameObjectsList->GetHandle(), 0, &nameCol);

// 	LV_COLUMN behaviourCol;
// 	behaviourCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
// 	behaviourCol.pszText = "Behaviour";
// 	behaviourCol.cx = 130;
// 
// 	ListView_InsertColumn(gameObjectsList->GetHandle(), 1, &behaviourCol);

	m_gameObjectsTab.RegisterControl(createButton);
	m_gameObjectsTab.RegisterControl(deleteButton);
	m_gameObjectsTab.RegisterControl(gameObjectsList);

}