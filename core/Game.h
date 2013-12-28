#pragma once

#include "GameObject.h"

#include "InputModule.h"
#include "GameObjectsModule.h"
#include "SceneModule.h"
#include "AudioModule.h"
#include "GraphicsModule.h"

namespace mye
{

	namespace core
	{

		class Game
		{

		public:

			Game(InputModule *input,
				 GameObjectsModule *gameobjects,
				 SceneModule *scene,
				 GraphicsModule *graphics,
				 AudioModule *audio);

			~Game(void);

			virtual void Run(void);

			InputModule* GetInputModule(void);
			GameObjectsModule* GetGameObjectsModule(void);
			SceneModule* GetSceneModule(void);
			GraphicsModule* GetGraphicsModule(void);
			AudioModule* GetAudioModule(void);

		private:

			InputModule *_input;
			GameObjectsModule *_gameobjects;
			SceneModule *_scene;
			GraphicsModule *_graphics;
			AudioModule *_audio;

		};

	}
}
