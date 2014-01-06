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

		private:

			InputModule *_input;
			GameObjectsModule *_gameobjects;
			SceneModule *_scene;
			GraphicsModule *_graphics;
			AudioModule *_audio;
			IScriptModule *_script;

		};

	}
}