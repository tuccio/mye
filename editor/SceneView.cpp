#include "SceneView.h"
#include "Globals.h"

#include <mye/win/Button.h>
#include <mye/win/Checkbox.h>
#include <mye/win/Edit.h>
#include <mye/win/ListView.h>
#include <mye/win/Utils.h>

#include <cstdlib>

#undef max

#define TAB_GAME_OBJECTS 0
#define TAB_PROPERTIES 1
#define TAB_TRANSFORM 2
#define TAB_RENDER 3
#define TAB_BEHAVIOUR 4
#define TAB_CAMERA 5

using namespace mye::dx11;
using namespace mye::win;
using namespace mye::math;
using namespace mye::core;

SceneView::SceneView(void) :
	m_bgColor(0.12f, 0.12f, 0.12f, 1.0f),
	m_initialized(false),
	m_tabs(m_tabsWindow),
	m_selected(nullptr),
	m_activeTab(-1),
	m_sensitivity(1.2f),
	m_dragSpeed(1.5f),
	m_sceneCamera(nullptr)
{

	m_camera.LookAt(
		Vector3f(0, 0, -5),
		Vector3f(0, 1, 0),
		Vector3f(0, 0, 0));

	m_camera.SetFarClipDistance(4096);

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

		m_tabsWindow.CreateChild(g_mainWindow);
		m_tabs.Create();

		m_tabsWindow.AddTabsListener(this);

		m_tabs.AddTab(TAB_GAME_OBJECTS, "Game Objects");
		m_tabs.AddTab(TAB_PROPERTIES, "Properties");
		m_tabs.AddTab(TAB_TRANSFORM, "Transform");
		m_tabs.AddTab(TAB_RENDER, "Render");
		m_tabs.AddTab(TAB_BEHAVIOUR, "Behaviour");
		m_tabs.AddTab(TAB_CAMERA, "Camera");

		_CreateGameObjectsTab();
		_CreateRenderTab();
		_CreatePropertiesTab();
		_CreateCameraTab();

		m_tabs.SelectTab(TAB_GAME_OBJECTS);
		m_gameObjectsTab.Show();

		SelectGameObject(nullptr);

		g_game.Init();

		m_timer.Start();

		m_initialized = true;

	}

	g_mainWindow.ResizeViews();

	m_tabs.Show();
	m_tabsWindow.Show();
	g_renderWindow.Show();

}

void SceneView::Deactivate(void)
{
	g_renderWindow.Hide();
	m_tabsWindow.Hide();
	m_tabs.Hide();
}

void SceneView::SetPosition(const mye::math::Vector2i &position)
{
	g_renderWindow.SetPosition(position);
}

void SceneView::SetSize(const mye::math::Vector2i &size)
{

	const int rightPanelWidth = 280;
	const int tabPadding = 8;
	const int tabHeight = 50;

	View::SetSize(size);

	Vector2i rightPanel(rightPanelWidth, size.y());
	Vector2i leftPanel(size.x() - rightPanelWidth, size.y());

	g_renderWindow.SetSize(leftPanel);
	m_camera.SetClipAspectRatio(leftPanel.x() / (leftPanel.y() > 0.0f ? leftPanel.y() : 1.0f));

 	m_tabsWindow.SetPosition(Vector2i(leftPanel.x(), 0));
 	m_tabsWindow.SetSize(rightPanel);

	Vector2i tabSize = Vector2i(rightPanel.x() - 2 * tabPadding, rightPanel.y() - tabHeight - tabPadding);
	Vector2i tabPosition = Vector2i(tabPadding, tabHeight);

	m_gameObjectsTab.SetSize(tabSize);
	m_gameObjectsTab.SetPosition(tabPosition);

	m_renderTab.SetSize(tabSize);
	m_renderTab.SetPosition(tabPosition);

	m_propertiesTab.SetSize(tabSize);
	m_propertiesTab.SetPosition(tabPosition);

	m_cameraTab.SetSize(tabSize);
	m_cameraTab.SetPosition(tabPosition);

	auto goList = m_controls.find("GOTGameObjectsList");

	if (goList != m_controls.end())
	{
		goList->second->SetSize(Vector2i(rightPanel.x() - 2 * tabPadding, rightPanel.y() - 24 - tabPadding));
	}

	m_tabs.SetSize(rightPanel);

	m_tabsWindow.Update();
	m_tabs.Update();

}

void SceneView::SetBackgroundColor(const mye::core::ColorRGBA &rgba)
{
	m_bgColor = rgba;
}

DX11Window& SceneView::GetRenderWindow(void)
{
	return g_renderWindow;
}

void SceneView::OnTabShow(int index)
{

	switch (index)
	{

	case TAB_GAME_OBJECTS:
		m_gameObjectsTab.Show();
		break;

	case TAB_PROPERTIES:
		_FillPropertiesTab(m_selected);
		m_propertiesTab.Show();
		break;

	case TAB_TRANSFORM:
		break;

	case TAB_RENDER:
		m_renderTab.Show();
		break;

	case TAB_BEHAVIOUR:
		break;

	case TAB_CAMERA:
		m_cameraTab.Show();
		break;

	default:
		break;

	}

	m_activeTab = index;
	m_tabs.Update();

}

void SceneView::OnTabHide(int index)
{
	switch (index)
	{

	case TAB_GAME_OBJECTS:
		m_gameObjectsTab.Hide();
		break;

	case TAB_PROPERTIES:
		m_propertiesTab.Hide();
		break;

	case TAB_TRANSFORM:
		break;

	case TAB_RENDER:
		m_renderTab.Hide();
		break;

	case TAB_BEHAVIOUR:
		break;

	case TAB_CAMERA:
		m_cameraTab.Hide();
		break;

	default:
		break;

	}
}

void SceneView::Update(void)
{

	bool needUpdate = false;

	const Mouse *mouse = g_input.GetMouse();
	const Keyboard *keyboard = g_input.GetKeyboard();

	Vector2f mousePosition = mouse->GetPosition();
	Vector2i windowLeftTop = g_renderWindow.GetPosition();
	Vector2i windowRightBottom = windowLeftTop + g_renderWindow.GetSize();

	if (mousePosition.x() >= windowLeftTop.x() &&
		mousePosition.x() <= windowRightBottom.x() &&
		mousePosition.y() >= windowLeftTop.y() &&
		mousePosition.y() <= windowRightBottom.y())
	{

		if (keyboard->IsPressed(MYE_VK_LCTRL))
		{

			if (mouse->IsPressed(MYE_VK_MOUSE_LEFT))
			{

				Vector2f delta = mouse->GetDelta() * m_sensitivity;

				m_camera.Yaw(Arctangent(delta.x()));
				m_camera.Pitch(Arctangent(delta.y()));

			}
			else if (mouse->IsPressed(MYE_VK_MOUSE_RIGHT))
			{

				Vector2f delta = mouse->GetDelta() * m_dragSpeed * .015f;

				Vector3f p = m_camera.GetPosition()
					+ m_camera.Right() * delta.x()
					+ m_camera.Up() * (- delta.y());

				m_camera.SetPosition(p);

			}
			else if (mouse->IsPressed(MYE_VK_MOUSE_MIDDLE))
			{

				Vector2f delta = mouse->GetDelta() * m_dragSpeed * 0.015f;

				Vector3f p = m_camera.GetPosition()
					+ m_camera.Right() * delta.x()
					+ m_camera.Forward() * (- delta.y());

				m_camera.SetPosition(p);

			}

		}
		else if (keyboard->IsPressed(MYE_VK_LALT) &&
			m_selected &&
			m_selected->GetRenderComponent())
		{

			mye::math::AABBf aabb = m_selected->GetAABB();

			TransformComponent *tc = m_selected->GetTransformComponent();

			if (mouse->IsPressed(MYE_VK_MOUSE_LEFT))
			{

				Vector2f delta = mouse->GetDelta() * m_sensitivity;

				tc->GetOrientation();


			}
			else if (mouse->IsPressed(MYE_VK_MOUSE_RIGHT))
			{

				Vector2f delta = mouse->GetDelta() * m_dragSpeed * .015f;

				Vector3f p = tc->GetPosition()
					+ tc->Right() * delta.x()
					+ tc->Up() * (- delta.y());

				tc->SetPosition(p);

				needUpdate = true;

			}
			else if (mouse->IsPressed(MYE_VK_MOUSE_MIDDLE))
			{

				Vector2f delta = mouse->GetDelta() * m_dragSpeed * .015f;

				Vector3f p = tc->GetPosition()
					+ tc->Right() * delta.x()
					+ tc->Forward() * (- delta.y());

				tc->SetPosition(p);

				needUpdate = true;

			}
			else if (float delta = mouse->GetWheelDelta())
			{

				tc->SetScale(tc->GetScale() + Vector3f(delta * 0.25f));

			}

			g_scene.MoveGameObject(m_selected->GetHandle(), aabb);

		}

	}

	static Milliseconds lastUpdate = 0;
	lastUpdate = lastUpdate + m_timer.Lap();

	if (needUpdate && lastUpdate > 250)
	{
		_FillPropertiesTab(m_selected);
		lastUpdate = 0;
	}

}

void SceneView::Render(void)
{

	g_device.GetImmediateContext()->ClearRenderTargetView(
		g_renderWindow.GetRenderTargetView(),
		m_bgColor.Data());

	g_device.GetImmediateContext()->ClearDepthStencilView(
		g_renderWindow.GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1,
		0);

	CameraComponent *sceneCamera = g_scene.GetCamera(); 
	CameraComponent *camera = &m_camera;

	g_scene.SetCamera(camera);

	if (camera)
	{

		DX11ConstantBuffer mvpBuffer(nullptr, "", nullptr, g_renderWindow.GetDevice());
		mvpBuffer.Create(sizeof(float) * 16, Matrix4f(1.0f).Data());

		SceneModule::ObjectsList objects = g_scene.GetVisibleObjects();

		Matrix4f viewProjection = camera->GetProjectionMatrix() *
			camera->GetViewMatrix();

		for (GameObject *object : objects)
		{

			RenderComponent *rc = object->GetRenderComponent();

			if (rc)
			{

				TransformComponent *tc = object->GetTransformComponent();
				Model *model = rc->GetModel().Cast<Model>();

				if (model)
				{

					model->Load();

					mvpBuffer.Bind(PIPELINE_VERTEX_SHADER, 0);
					mvpBuffer.SetData((viewProjection *
						tc->GetWorldMatrix()).Data());

					DX11VertexBuffer vertexBuffer(nullptr, "", nullptr, g_renderWindow.GetDevice());

					vertexBuffer.Create(model);
					vertexBuffer.Bind();

					g_renderWindow.GetDevice().GetImmediateContext()->
						IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					g_renderWindow.GetDevice().GetImmediateContext()->
						Draw(vertexBuffer.GetVerticesCount(), 0);

					vertexBuffer.Destroy();

				}

			}

		}

		g_scene.SetCamera(sceneCamera);

		mvpBuffer.Destroy();

	}

	

	g_renderWindow.GetSwapChain()->Present(1, 0);

	//g_game.GetGraphicsModule()->Render();

}

/* Tabs */

void SceneView::_AddGameObject(GameObjectHandle hObj)
{

	GameObject *gameObject = g_gameObjectsModule.Get(hObj);

	ListView *gameObjectsList = static_cast<ListView*>(m_controls["GOTGameObjectsList"]);
	int i = ListView_GetItemCount(gameObjectsList->GetHandle());

	LV_ITEM item;
	ZeroMemory(&item, sizeof(LV_ITEM));
	item.iItem = i;
	item.cchTextMax = 64;
	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.pszText =  (gameObject->GetName().Length() > 0 ? gameObject->GetName().CString() : "<Anon>");
	item.lParam = (LPARAM) gameObject;

	ListView_InsertItem(gameObjectsList->GetHandle(), &item);
	SelectGameObject((GameObject*) (item.lParam));

}

void SceneView::_CreateGameObjectsTab(void)
{

	m_gameObjectsTab.CreateChild(m_tabs.GetHandle());

	ListView *gameObjectsList = new ListView;
	m_controls["GOTGameObjectsList"] = gameObjectsList;

	gameObjectsList->Create(
		m_gameObjectsTab,
		Vector2i(0, 28));

	Button *createButton = new Button;
	m_controls["GOTCreateButton"] = createButton;

	createButton->Create(
		m_gameObjectsTab,
		"Create",
		[this, gameObjectsList] (void) -> void
		{

			GameObjectHandle hObj = g_gameObjectsModule.Create();

			_AddGameObject(hObj);
			g_scene.AddGameObject(hObj);

		},
		Vector2i(0, 0),
		Vector2i(180, 24));

	Button *deleteButton = new Button;
		m_controls["GOTDeleteButton"] = deleteButton;

	deleteButton->Create(
			m_gameObjectsTab,
			"Delete",
			[this, gameObjectsList] (void) -> void
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

					GameObject *object = reinterpret_cast<GameObject*>(item.lParam);
					GameObjectHandle hObj = object->GetHandle();

					if (object->GetRenderComponent())
					{
						g_scene.RemoveGameObject(hObj);
					}

					ListView_DeleteItem(gameObjectsList->GetHandle(), selected);
					SelectGameObject(nullptr);

					g_gameObjectsModule.Destroy(hObj);

				}

			}

		},
			Vector2i(184, 0),
			Vector2i(80, 24));

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

	gameObjectsList->AddListener(this);

}

void SceneView::OnListViewEvent(ListView *listView, unsigned int code, LPARAM lParam)
{

	switch (code)
	{

	case NM_DBLCLK:

		{
			int selected = ListView_GetNextItem(listView->GetHandle(), -1, LVNI_SELECTED);

			if (selected >= 0)
			{

				LV_ITEM item;
				ZeroMemory(&item, sizeof(LV_ITEM));
				item.mask = LVIF_PARAM;
				item.iItem = selected;

				BOOL itemSelected = ListView_GetItem(listView->GetHandle(), &item);

				if (itemSelected && item.lParam)
				{
					GameObject *selectedObject = ((GameObject*) (item.lParam));
					SelectGameObject(selectedObject);
				}

			}
		}
		break;

	default:
		break;

	}

}

void SceneView::SelectGameObject(GameObject *selectedObject)
{
	
	/* Update tabs */

	_FillRenderTab(selectedObject);
	_FillPropertiesTab(selectedObject);
	_FillCameraTab(selectedObject);

	m_selected = selectedObject;

}

