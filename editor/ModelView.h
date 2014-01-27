#pragma once

#include "View.h"

#include <mye/d3d11/DX11Device.h>
#include <mye/d3d11/DX11Window.h>
#include <mye/d3d11/DX11VertexBuffer.h>
#include <mye/d3d11/DX11ConstantBuffer.h>

#include <mye/win/Toolbar.h>

#include <mye/core/AssimpModelLoader.h>
#include <mye/core/ColorRGBA.h>
#include <mye/core/Camera.h>
#include <mye/core/Model.h>

#include <mye/math/Math.h>

class ModelView :
	public View
{

public:

	ModelView(mye::dx11::DX11Device &device);
	~ModelView(void);

	void Activate(void);
	void Deactivate(void);

	void SetPosition(const mye::math::Vector2i &position);
	void SetSize(const mye::math::Vector2i &size);

	void Update(void);
	void Render(void);

	void SetBackgroundColor(const mye::core::ColorRGBA &rgba);

	mye::dx11::DX11Window& GetWindow(void);

private:

	void UpdateBuffer(void);

	/* View members */

	mye::dx11::DX11Window m_window;
	mye::dx11::DX11Device &m_device;

	mye::core::ColorRGBA m_bgColor;

	mye::win::Toolbar m_toolbar;

	bool m_initialized;

	/* Input */

	enum InputMode
	{
		MODELVIEW_INPUT_TRANSFORM
	} m_inputMode;

	/* Content members */

	mye::core::AssimpModelLoader m_modelLoader;

	mye::core::ResourceHandle m_model;
	mye::math::Transformf m_localTransform;
	mye::math::Transformf m_worldTransform;

	mye::dx11::DX11VertexBuffer m_vbuffer;

	mye::core::Camera m_camera;
	mye::dx11::DX11ConstantBuffer m_mvpBuffer;

};

