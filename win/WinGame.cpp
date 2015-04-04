#include "WinGame.h"
#include "Utils.h"

using namespace std;
using namespace mye::core;
using namespace mye::win;

WinGame::WinGame(InputModule *input,
	GameObjectsModule *gameobjects,
	SceneModule *scene,
	PhysicsModule *physics,
	GraphicsModule *graphics,
	AudioModule *audio,
	ScriptModule *script) :
Game(input,
	gameobjects,
	scene,
	physics,
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

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		FloatSeconds dt(m_timer.Lap());

		m_script->Preupdate(dt);
		m_input->Preupdate();
		//m_objects->Preupdate(dt);

		/*m_gameobjects->Update()
		m_gameobjects->FinalizeUpdate();*/

		for (auto it = m_gameobjects->begin(); it != m_gameobjects->end(); it++)
		{
			m_script->Update(it);
		}

		m_physics->Update(dt);
		//m_scene->Update();
		m_graphics->Render();

		m_gameobjects->FinalizeUpdate();

	}
	while (msg.message != WM_QUIT);

	m_timer.Stop();

}

void Quit(void)
{
	PostQuitMessage(WM_QUIT);
}

void WinGame::RuntimeError(const mye::core::String &error)
{
	ShowErrorBox(error);
}

void WinGame::MainWindowListener::OnDestroy(IWindow *window)
{
	Game::GetSingleton().Quit();
}

