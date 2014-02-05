#include "WinGame.h"
#include "Utils.h"

using namespace std;
using namespace mye::core;
using namespace mye::win;

WinGame::WinGame(InputModule *input,
	GameObjectsModule *gameobjects,
	SceneModule *scene,
	GraphicsModule *graphics,
	AudioModule *audio,
	ScriptModule *script) :
Game(input,
	gameobjects,
	scene,
	graphics,
	audio,
	script)
{
// 	m_mainWindow.AddListener(&m_mainWindowListener);
// 	m_mainWindow.Create();
}

WinGame::~WinGame(void)
{
/*	m_mainWindow.Destroy();*/
}

void WinGame::Run(void)
{

// 	if (m_mainWindow.Exists())
// 	{
// 		m_mainWindow.Create();
// 	}
// 
// 	m_mainWindow.Show();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	m_timer.Start();

	do
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		FloatSeconds dt(m_timer.Lap());

		m_gameobjects->Update(dt);
		//m_physics->Update(dt);
		//m_scene->Update();
		m_graphics->Render();

		m_input->ResetDeltas();

	}
	while (msg.message != WM_QUIT);

	m_timer.Stop();

}

void Quit(void)
{
	PostQuitMessage(WM_QUIT);
}

void WinGame::RuntimeError(const std::string &error)
{
	ShowErrorBox(error);
}

void WinGame::MainWindowListener::OnDestroy(IWindow *window)
{
	Game::GetSingleton().Quit();
}

