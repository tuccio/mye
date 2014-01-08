#pragma once

#include <mye/core/Game.h>

#include "RenderWindow.h"

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

			mye::core::IWindow& GetMainWindow(void);

			void Run(void);

			~WinGame(void);

		private:

			RenderWindow _mainWindow;

		};

	}

}