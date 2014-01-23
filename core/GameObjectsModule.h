#pragma once

#include "Module.h"
#include "GameObject.h"

namespace mye
{

	namespace core
	{

		class GameObjectsModule : public Module,
								  public GameObjectsManager
		{

		public:

			void Update(FloatSeconds dt);

		};

	}

}