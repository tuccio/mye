#pragma once

#include "LuaModule.h"

#include <mye/core/GameObject.h>
#include <mye/core/Game.h>
#include <mye/core/Component.h>
#include <mye/core/GameObjectsManager.h>
#include <mye/core/Resource.h>

#include <functional>
#include <vector>

#include <lua++11\lua++11.h>

namespace mye
{

	namespace lua
	{

		void BindGameObjectHandle(luapp11::State state);

	}

}