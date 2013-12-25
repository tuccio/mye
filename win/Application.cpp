#include "Application.h"

using namespace mye::win;

Application::Application(void)
{
	_mainWindow.Create();
}


Application::~Application(void)
{
	_mainWindow.Destroy();
}

Window& Application::GetMainWindow(void)
{
	return _mainWindow;
}

const Window& Application::GetMainWindow(void) const
{
	return _mainWindow;
}

void Application::Run(void)
{

	if (_mainWindow.Exists())
	{
		_mainWindow.Create();
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	do
	{

		//_mainWindow.PeekAndDispatch(msg);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		static unsigned int _lastTime = GetTickCount();
		unsigned int currentTime = GetTickCount();

		float dt = (currentTime  - _lastTime) / 1000.0f;

// 		_game->Update(dt);
// 		_game->Render();

		_lastTime = currentTime;

		

	}
	while (msg.message != WM_QUIT);

}