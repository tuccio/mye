#pragma once

#include "View.h"

#include <mye/d3d11/DX11Device.h>
#include <mye/d3d11/DX11Window.h>

#include <mye/core/ColorRGBA.h>

class SceneView :
	public View
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

	void Update(void);
	void Render(void);

private:

	mye::dx11::DX11Window m_window;
	mye::dx11::DX11Device &m_device;

	mye::core::ColorRGBA m_bgColor;

	bool m_initialized;

};

