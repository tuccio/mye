#pragma once

#include "LuaEnvironment.h"

#include <luabind/luabind.hpp>
#include <mye/core/Component.h>

namespace mye
{

	namespace lua
	{

		void BindGameObjectHandle(lua_State *L);

		std::string __goh_getname(const luabind::object &o);

		luabind::object __goh_getcomponent(const luabind::object &o,
										   const std::string &name);

		void __goh_addcomponent(const luabind::object &o,
								const std::string &name,
								const mye::core::Component &component);

	}

}