#pragma once

#include <mye/core/IWindow.h>

class MainWindowListener :
	public mye::core::IWindow::Listener
{

public:

	MainWindowListener(void);
	~MainWindowListener(void);

	void OnResize(mye::core::IWindow *window,
		const Eigen::Vector2i &size);

};

