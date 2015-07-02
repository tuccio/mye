#pragma once

#include "GameObject.h"

#include "EventManager.h"
#include "InputModule.h"
#include "GameObjectsModule.h"
#include "SceneModule.h"
#include "PhysicsModule.h"
#include "AudioModule.h"
#include "GraphicsModule.h"
#include "ScriptModule.h"

#include "IWindow.h"
#include "Singleton.h"
#include "Time.h"

#include <typeindex>
#include <exception>
#include <list>

namespace mye
{

	namespace core
	{

		class Game :
			public Singleton<Game>
		{

		public:

			
			Game(InputModule       * input,
				 GameObjectsModule * gameobjects,
				 SceneModule       * scene,
				 PhysicsModule     * physics,
				 GraphicsModule    * graphics,
				 AudioModule       * audio,
				 ScriptModule      * script);

			~Game(void);

			bool Init(void);
			virtual void Run(void);
			virtual void Quit(void);

			InputModule       * GetInputModule(void);
			GameObjectsModule * GetGameObjectsModule(void);
			SceneModule       * GetSceneModule(void);
			PhysicsModule     * GetPhysicsModule(void);
			GraphicsModule    * GetGraphicsModule(void);
			AudioModule       * GetAudioModule(void);
			ScriptModule      * GetScriptModule(void);

			virtual void RuntimeError(const String &error);

			void ImportScene(const String &file, std::list<GameObject*> *allocatedObjects = nullptr);
			void ExportScene(const String &file);

		protected:

			InputModule       * m_input;
			GameObjectsModule * m_gameobjects;
			SceneModule       * m_scene;
			PhysicsModule     * m_physics;
			GraphicsModule    * m_graphics;
			AudioModule       * m_audio;
			ScriptModule      * m_script;

			EventManager      * m_eventManager;

			LapStopWatch m_timer;

		};

	}
}