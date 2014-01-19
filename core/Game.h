#pragma once

#include "GameObject.h"

#include "InputModule.h"
#include "GameObjectsModule.h"
#include "SceneModule.h"
#include "AudioModule.h"
#include "GraphicsModule.h"
#include "ScriptModule.h"

#include "IWindow.h"
#include "Singleton.h"

#include <typeindex>
#include <exception>

namespace mye
{

	namespace core
	{

		class Game :
			public Singleton<Game>
		{

		public:

			
			Game(InputModule *input,
				 GameObjectsModule *gameobjects,
				 SceneModule *scene,
				 GraphicsModule *graphics,
				 AudioModule *audio,
				 IScriptModule *script);

			~Game(void);

			bool Init(void);
			virtual void Run(void);
			virtual void Quit(void);

			virtual IWindow& GetMainWindow(void) = 0;

			InputModule* GetInputModule(void);
			GameObjectsModule* GetGameObjectsModule(void);
			SceneModule* GetSceneModule(void);
			GraphicsModule* GetGraphicsModule(void);
			AudioModule* GetAudioModule(void);
			IScriptModule* GetScriptModule(void);

			virtual void RuntimeError(const std::string &error);

		private:

			InputModule *m_input;
			GameObjectsModule *m_gameobjects;
			SceneModule *m_scene;
			GraphicsModule *m_graphics;
			AudioModule *m_audio;
			IScriptModule *m_script;

		};

	}
}