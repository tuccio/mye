#include "SceneView.h"
#include "Globals.h"

#include <cstdlib>

#undef max

using namespace mye::dx11;
using namespace mye::win;

SceneView::SceneView(DX11Device &device) :
	m_window(device),
	m_device(device),
	m_bgColor(0.12f, 0.12f, 0.12f, 1.0f),
	m_initialized(false)
{
}


SceneView::~SceneView(void)
{
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

			MessageBox(NULL,
				"Error while initiating rendering window\nConsult logs for more details",
				"Error", MB_OK);

			exit(1);

		}

		m_initialized = true;

	}

	m_window.Show();

}

void SceneView::Deactivate(void)
{
	m_window.Hide();
}

void SceneView::SetPosition(const mye::math::Vector2i &position)
{
	m_window.SetPosition(position);
}

void SceneView::SetSize(const mye::math::Vector2i &size)
{
	View::SetSize(size);
	m_window.SetSize(size);
}

void SceneView::SetBackgroundColor(const mye::core::ColorRGBA &rgba)
{
	m_bgColor = rgba;
}

void SceneView::Update(void)
{

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