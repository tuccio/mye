#pragma once

#include "View.h"

#include <mye/d3d11/DX11Device.h>

class SceneView :
	public View
{

public:

	SceneView(void);
	~SceneView(void);

	void Resize(const Eigen::Vector2i &size);
	void Update(void);
	void Render(void);

private:

	mye::dx11::DX11Device m_device;

};

