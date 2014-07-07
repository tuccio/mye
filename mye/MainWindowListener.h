#pragma once

#include <mye/core/IWindow.h>
#include <mye/core/Game.h>

#include <mye/core/SceneModule.h>

class MainWindowListener :
	public mye::core::IWindow::Listener,
	public mye::core::SceneCameraListener
{

public:

	MainWindowListener(void);
	~MainWindowListener(void);

	void OnCreate(mye::core::IWindow * window);
	void OnDestroy(mye::core::IWindow *window);
	void OnResize(mye::core::IWindow *window, const mye::math::Vector2i &size);

	void OnCameraChange(mye::core::Camera *oldCamera, mye::core::Camera *newCamera);

private:

	mye::core::Game *m_game;

};

