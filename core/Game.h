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

			static GameObjectHandle CreateGameObject(void);
			static GameObject* GetGameObject(const GameObjectHandle &hObj);
			static void DestroyGameObject(const GameObjectHandle &hObj);
			static bool IsGameObject(const GameObjectHandle &hObj);

		private:

			Game(void);

			static GameObjectsManager _goManager;

		};

	}
}
