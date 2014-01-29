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

	void OnCommand(unsigned int id, unsigned int code);

	void OnMenuSelected(mye::win::IDGenerator::ID id);

};

