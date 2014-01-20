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

class ModelView :
	public View
{

public:

	ModelView(mye::dx11::DX11Device &device);
	~ModelView(void);

	void Activate(void);
	void Deactivate(void);

	void SetPosition(const Eigen::Vector2i &position);
	void SetSize(const Eigen::Vector2i &size);

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

	/* Content members */

	mye::core::AssimpModelLoader m_modelLoader;
	mye::core::ResourceHandle m_model;
	Eigen::Matrix4f m_transform;
	mye::dx11::DX11VertexBuffer m_vbuffer;

	mye::core::Camera m_camera;
	mye::dx11::DX11ConstantBuffer m_mvpBuffer;

};

