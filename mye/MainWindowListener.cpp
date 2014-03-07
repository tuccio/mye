#include "MainWindowListener.h"

using namespace mye::core;
using namespace mye::math;

MainWindowListener::MainWindowListener(Game *game) :
	m_game(game)
{

	IWindow *window = game->GetGraphicsModule()->GetWindow();

	if (window)
	{
		OnCreate(window);
	}

	game->GetSceneModule()->AddCameraListener(this);

}


MainWindowListener::~MainWindowListener(void)
{
}

void MainWindowListener::OnDestroy(IWindow *window)
{
	
	m_game->Quit();

}

void MainWindowListener::OnCreate(mye::core::IWindow * window)
{
	OnResize(window, window->GetSize());
	window->AddListener(this);
}

void MainWindowListener::OnResize(IWindow *window, const Vector2i &size)
{

	CameraComponent *camera = m_game->GetSceneModule()->GetCamera();

	if (camera)
	{
		camera->SetClipAspectRatio((float) size.x() / size.y());
	}

}

void MainWindowListener::OnCameraChange(mye::core::CameraComponent *oldCamera,
										mye::core::CameraComponent *newCamera)
{

	auto size = m_game->GetGraphicsModule()->GetWindow()->GetSize();

	newCamera->SetClipAspectRatio((float) size.x() / size.y());

}