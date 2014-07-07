#include "MainWindowListener.h"

using namespace mye::core;
using namespace mye::math;

MainWindowListener::MainWindowListener(void)
{

	m_game = Game::GetSingletonPointer();

	IWindow *window = m_game->GetGraphicsModule()->GetWindow();

	if (window)
	{
		OnCreate(window);
	}

	m_game->GetSceneModule()->AddCameraListener(this);

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

}

void MainWindowListener::OnResize(IWindow *window, const Vector2i &size)
{

	Camera *camera = m_game->GetSceneModule()->GetCamera();

	if (camera)
	{
		camera->SetClipAspectRatio((float) size.x() / size.y());
	}

}

void MainWindowListener::OnCameraChange(mye::core::Camera *oldCamera,
										mye::core::Camera *newCamera)
{

	// TODO: This should be done somewhere else

	auto size = m_game->GetGraphicsModule()->GetWindow()->GetSize();

	newCamera->SetClipAspectRatio((float) size.x() / size.y());

}