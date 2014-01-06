#pragma once

#include "LuaModule.h"

#include <luabind/luabind.hpp>

#include <mye/core/GameObject.h>
#include <mye/core/Component.h>

#include <vector>

namespace mye
{

	namespace lua
	{

		void BindGameObjectHandle(lua_State *L);

		std::string __goh_getname(const mye::core::GameObjectHandle &hObj);

		luabind::object __goh_getcomponent(const mye::core::GameObjectHandle &hObj,
										   const std::string &name);

		void __goh_addcomponent(const mye::core::GameObjectHandle &hObj,
								const std::string &name,
								const mye::core::Component &component);


		luabind::object __goh_getparent(const mye::core::GameObjectHandle &hObj);
		void __goh_setparent(const mye::core::GameObjectHandle &hObj, const luabind::object &oHandle);

		luabind::object __goh_getchildren(const mye::core::GameObjectHandle &hObj);

		std::string __goh_tostring(const mye::core::GameObjectHandle &hObj);

	}

}