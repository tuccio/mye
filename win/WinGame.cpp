#include "WinGame.h"

using namespace std;
using namespace mye::core;
using namespace mye::win;

WinGame::WinGame(InputModule *input,
	GameObjectsModule *gameobjects,
	SceneModule *scene,
	GraphicsModule *graphics,
	AudioModule *audio,
	IScriptModule *script) :
Game(input,
	gameobjects,
	scene,
	graphics,
	audio,
	script)
{
	m_mainWindow.AddListener(&m_mainWindowListener);
	m_mainWindow.Create();
}

WinGame::~WinGame(void)
{
	m_mainWindow.Destroy();
}

mye::core::IWindow& WinGame::GetMainWindow(void)
{
	return m_mainWindow;
}

void WinGame::Run(void)
{

	if (m_mainWindow.Exists())
	{
		m_mainWindow.Create();
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	do
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		static unsigned int _lastTime = GetTickCount();
		unsigned int currentTime = GetTickCount();

		float dt = (currentTime  - _lastTime) / 1000.0f;

		/*
		Update(dt);
		Render();
		*/

		_lastTime = currentTime;



	}
	while (msg.message != WM_QUIT);

}

void WinGame::MainWindowListener::OnDestroy(IWindow *window)
{
	PostQuitMessage(WM_QUIT);
}