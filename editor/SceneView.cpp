#include "SceneView.h"
#include "Globals.h"

#include <cstdlib>

#undef max

SceneView::SceneView(void)
{
}


SceneView::~SceneView(void)
{
}

void SceneView::Resize(const Eigen::Vector2i &size)
{
	g_childWindow.SetPosition(Eigen::Vector2i(g_leftToolbarWidth, 0));
	g_childWindow.SetSize(Eigen::Vector2i(size.x() - g_leftToolbarWidth, size.y()));
}

void SceneView::Update(void)
{

}

void SceneView::Render(void)
{

	g_device.GetImmediateContext()->ClearRenderTargetView(
		g_childWindow.GetRenderTargetView(),
		Eigen::Vector4f(0.12f, 0.12f, 0.12f, 1.0f).data());

	g_device.GetImmediateContext()->ClearDepthStencilView(
		g_childWindow.GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1,
		0);

	g_childWindow.GetSwapChain()->Present(1, 0);

}