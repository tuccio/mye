#include "MainWindowListener.h"
#include "Globals.h"

MainWindowListener::MainWindowListener(void)
{
}


MainWindowListener::~MainWindowListener(void)
{
}

void MainWindowListener::OnResize(mye::core::IWindow *window,
								  const Eigen::Vector2i &size)
{

	g_renderWindow.SetPosition(Eigen::Vector2i(g_toolbarWidth, 0));
	g_renderWindow.SetSize(Eigen::Vector2i(size.x() - g_toolbarWidth, size.y()));

	/* Fill render window with orange just to see it */

	HDC hDC = g_renderWindow.GetDC();
	RECT rect;
	GetClientRect(g_renderWindow.GetHandle(), &rect);
	FillRect(hDC, &rect, CreateSolidBrush(RGB(0xFF, 0x8F, 0x00)));
	g_renderWindow.ReleaseDC(hDC);

}