#pragma once

#include "GameObject.h"

namespace mye
{

	namespace core
	{

		class Game
		{

		public:

			~Game(void);

			/* Game objects */

			static GameObjectHandle CreateGameObject(void);
			static GameObjectHandle CreateGameObject(const std::string &classname);

			static GameObject* GetGameObject(const GameObjectHandle &hObj);
			static void DestroyGameObject(const GameObjectHandle &hObj);

			static bool IsGameObject(const GameObjectHandle &hObj);

		private:

			Game(void);

			static GameObjectsManager _goManager;

		};

	}
}
