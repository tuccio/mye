#pragma once

#include <mye/core/Game.h>
#include <mye/win/Window.h>

namespace mye
{

	namespace win
	{

		class WinGame : public mye::core::Game
		{

		public:

			WinGame(mye::core::InputModule *input,
				mye::core::GameObjectsModule *gameobjects,
				mye::core::SceneModule *scene,
				mye::core::GraphicsModule *graphics,
				mye::core::AudioModule *audio,
				mye::core::IScriptModule *script);

			~WinGame(void);

			mye::core::IWindow& GetMainWindow(void);

			void Run(void);

			void RuntimeError(const std::string &error);

		private:

			Window m_mainWindow;

			class MainWindowListener :
				public Window::Listener
			{
			public:
				void OnDestroy(mye::core::IWindow *window);
			} m_mainWindowListener;

		};

	}

}