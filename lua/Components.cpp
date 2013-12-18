#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include <mye/core/GameObject.h>
#include <mye/core/Game.h>

#include "ComponentHandler.h"

#include <string>

using namespace mye::core;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		int RegisterNumberComponent(lua_State *L)
		{


// 			luabind::object chTable = luabind::newtable(L);
// 			
// 			chTable["hObj"] = luabind::globals(L)["GameObjectHandle"];
// 
// 			luabind::object chMTable = luabind::newtable(L);
// 
// 			luabind::object getFunc = luabind::make_function(L, &);
// 
// 			luabind::setmetatable(chTable, chMTable);


			return 0;

		}

		int RegisterComponents(lua_State *L)
		{

			return RegisterNumberComponent(L);

		}

	}

}