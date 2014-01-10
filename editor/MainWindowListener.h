#pragma once

#include <mye/core/IWindow.h>
#include <mye/win/WindowMenu.h>

class MainWindowListener :
	public mye::core::IWindow::Listener,
	public mye::win::WindowMenu::Listener
{

public:

	MainWindowListener(void);
	~MainWindowListener(void);

	void OnCreate(mye::core::IWindow * window);

	void OnDestroy(mye::core::IWindow * window);

	void OnResize(mye::core::IWindow *window,
		const Eigen::Vector2i &size);

	void OnMenuSelected(mye::win::IDGenerator::ID id);

};

