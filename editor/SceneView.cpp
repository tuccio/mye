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

/*
	HDC hDC = g_childWindow.GetDC();
	RECT rect;
	GetClientRect(g_childWindow.GetHandle(), &rect);

	HBRUSH hBrush = CreateSolidBrush(RGB(0xFF, 0xBF, 0x00));
	FillRect(hDC, &rect, hBrush);
	DeleteObject(hBrush);*/
	//g_childWindow.ReleaseDC(hDC);

}