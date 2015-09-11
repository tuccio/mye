#include "WinGame.h"
#include "Utils.h"

#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>

using namespace std;
using namespace mye::core;
using namespace mye::win;

static boost::condition_variable s_renderCondition;
static boost::mutex              s_renderMutex;
static bool                      s_renderReady = true;

inline void __RenderingThread(GraphicsModule * graphics)
{

	while (true)
	{

		graphics->Render();

		{
			boost::lock_guard<boost::mutex> lock(s_renderMutex);
			s_renderReady = true;
		}

		s_renderCondition.notify_one();

		boost::this_thread::interruption_point();

	}

}

WinGame::WinGame(InputModule       * input,
				 GameObjectsModule * gameobjects,
				 SceneModule       * scene,
				 PhysicsModule     * physics,
				 GraphicsModule    * graphics,
				 AudioModule       * audio,
				 ScriptModule      * script) :
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

	boost::thread renderingThread(__RenderingThread, m_graphics);

	do
	{

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		m_eventManager->Update();

		m_gameobjects->Preupdate();

		FloatSeconds dt(m_timer.Lap());

		m_script->Preupdate(dt);
		m_input->Preupdate();
		//m_objects->Preupdate(dt);

		/*m_gameobjects->Update()
		m_gameobjects->FinalizeUpdate();*/

		m_script->Update();

		m_physics->Update(dt);
		//m_scene->Update();

		boost::unique_lock<boost::mutex> lock(s_renderMutex);

		while (!s_renderReady)
		{
			s_renderCondition.wait(lock);
		}

		//m_graphics->Render();

	} while (msg.message != WM_QUIT);

	renderingThread.interrupt();
	renderingThread.join();

	m_timer.Stop();

}

void WinGame::Quit(void)
{
	Game::Quit();
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

