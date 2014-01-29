#pragma once

#include "View.h"

#include <mye/d3d11/DX11Device.h>
#include <mye/d3d11/DX11Window.h>

#include <mye/core/ColorRGBA.h>
#include <mye/core/GameObject.h>

#include <mye/win/Tabs.h>
#include <mye/win/Control.h>

#include <map>

class SceneView :
	public View,
	public mye::win::Tabs::Listener
{

public:

	SceneView(mye::dx11::DX11Device &device);
	~SceneView(void);

	void Activate(void);
	void Deactivate(void);

	void SetPosition(const mye::math::Vector2i &position);
	void SetSize(const mye::math::Vector2i &size);

	void Resize(const mye::math::Vector2i &size);

	void SetBackgroundColor(const mye::core::ColorRGBA &rgba);

	void OnTabShow(int index);
	void OnTabHide(int index);

	void Update(void);
	void Render(void);

private:

	void _CreateGameObjectsTab(void);

	mye::dx11::DX11Window m_window;
	mye::dx11::DX11Device &m_device;

	mye::win::Tabs m_tabs;
	mye::win::Window m_tabsWindow;

	mye::core::ColorRGBA m_bgColor;

	mye::win::Window m_gameObjectsTab;

	std::map<const std::string, mye::win::Control*> m_controls;

	mye::core::GameObject *m_selected;
	int m_activeTab;

	bool m_initialized;

};

