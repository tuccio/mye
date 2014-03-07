#pragma once

#include "Module.h"
#include "GameObject.h"
#include "GameObjectsManager.h"

#include <deque>

namespace mye
{

	namespace core
	{

		class GameObjectsModule : public Module,
								  public GameObjectsManager
		{

		public:			

			void PostDestroy(GameObjectHandle hObj);

			void Update(FloatSeconds dt);
			void FinalizeUpdate(void);

		private:

			std::deque<GameObjectHandle> m_destructionQueue;

		};

	}

}