#include "WinGame.h"

using namespace mye::win;
using namespace mye::core;

WinGame::WinGame(InputModule *input,
				 GameObjectsModule *gameobjects,
				 SceneModule *scene,
				 GraphicsModule *graphics,
				 AudioModule *audio) :
	Game(input,
		 gameobjects,
		 scene,
		 graphics,
		 audio)
{
	_mainWindow.Create();
}

WinGame::~WinGame(void)
{
	_mainWindow.Destroy();
}

Window WinGame::GetMainWindow(void)
{
	return _mainWindow;
}

void WinGame::Run(void)
{

	if (_mainWindow.Exists())
	{
		_mainWindow.Create();
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