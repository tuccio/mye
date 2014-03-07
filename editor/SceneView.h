#pragma once

#include "View.h"

#include <mye/d3d11/DX11Device.h>
#include <mye/d3d11/DX11Window.h>

#include <mye/core/Camera.h>
#include <mye/core/ColorRGBA.h>
#include <mye/core/GameObject.h>

#include <mye/win/Tabs.h>
#include <mye/win/Control.h>
#include <mye/win/ListView.h>

#include <map>

class SceneView :
	public View,
	public mye::win::Tabs::Listener,
	public mye::win::ListView::Listener
{

public:

	SceneView(void);
	~SceneView(void);

	void Activate(void);
	void Deactivate(void);

	void SetPosition(const mye::math::Vector2i &position);
	void SetSize(const mye::math::Vector2i &size);

	void Resize(const mye::math::Vector2i &size);

	void SetBackgroundColor(const mye::core::ColorRGBA &rgba);

	void OnTabShow(int index);
	void OnTabHide(int index);

	void OnListViewEvent(mye::win::ListView *listView, unsigned int code, LPARAM lParam);

	void SelectGameObject(mye::core::GameObject *selectedObject);

	void Update(void);
	void Render(void);

	mye::dx11::DX11Window& GetRenderWindow(void);

private:

	friend class MainWindowListener;

	void _CreateGameObjectsTab(void);
	void _CreateRenderTab(void);
	void _CreatePropertiesTab(void);
	void _CreateCameraTab(void);
	void _CreateBehaviourTab(void);

	void _FillRenderTab(mye::core::GameObject *selectedObject);
	void _FillPropertiesTab(mye::core::GameObject *selectedObject);
	void _FillCameraTab(mye::core::GameObject *selectedObject);
	void _FillBehaviourTab(mye::core::GameObject *selectedObject);;

	void _AddGameObject(mye::core::GameObjectHandle hObj);

	mye::core::LapStopWatch m_timer;

	mye::core::CameraComponent m_camera;
	mye::core::CameraComponent *m_sceneCamera;

	mye::win::Tabs m_tabs;
	mye::win::Window m_tabsWindow;

	mye::core::ColorRGBA m_bgColor;

	mye::win::Window m_gameObjectsTab;
	mye::win::Window m_renderTab;
	mye::win::Window m_propertiesTab;
	mye::win::Window m_cameraTab;
	mye::win::Window m_behaviourTab;

	std::map<const std::string, mye::win::Control*> m_controls;

	mye::core::GameObject *m_selected;
	int m_activeTab;

	bool m_initialized;

	float m_sensitivity;
	float m_dragSpeed;

};

