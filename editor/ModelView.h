#pragma once

#include "View.h"

#include <mye/d3d11/DX11Device.h>
#include <mye/d3d11/DX11VertexBuffer.h>
#include <mye/d3d11/DX11ConstantBuffer.h>

#include <mye/win/Toolbar.h>

#include <mye/core/AssimpLoader.h>
#include <mye/core/Camera.h>
#include <mye/core/Model.h>

#include <mye/math/Geometry.h>

class ModelView :
	public View
{

public:

	ModelView(void);
	~ModelView(void);

	void Activate(void);
	void Deactivate(void);

	void SetPosition(const mye::math::Vector2i &position);
	void SetSize(const mye::math::Vector2i &size);

	void Update(void);
	void Render(void);

	void SetBackgroundColor(const mye::math::Vector4f &rgba);

	mye::win::Window& GetWindow(void);

private:

	void UpdateBuffer(void);

	/* View members */

	mye::math::Vector4f m_bgColor;

	mye::win::Toolbar m_toolbar;

	bool m_initialized;

	/* Input */

	enum InputMode
	{
		MODELVIEW_INPUT_TRANSFORM
	} m_inputMode;

	/* Content members */

	mye::core::ModelPointer m_model;
	mye::math::Transform m_transform;

	mye::dx11::DX11VertexBuffer m_vbuffer;
	mye::dx11::DX11VertexBuffer m_PlanetBuffer;

	mye::core::Camera m_camera;
	mye::dx11::DX11ConstantBuffer m_mvpBuffer;

};

