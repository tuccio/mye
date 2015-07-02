#pragma once

#include "Module.h"
#include "GameObject.h"
#include "GameObjectsManager.h"

#include "EventManager.h"

#include <deque>

namespace mye
{

	namespace core
	{

		class GameObjectsModule :
			public Module,
			public GameObjectsManager,
			public IEventListener
		{

		public:

			bool Init(void);
			void Shutdown(void);

			void PostDestroy(GameObjectHandle hObj);

			void Update(void);
			void FinalizeUpdate(void);
			
			void OnEvent(const IEvent * event);

		};

	}

}