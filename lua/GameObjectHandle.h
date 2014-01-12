#pragma once

#include "LuaModule.h"

#include <luabind/luabind.hpp>

#include <mye/core/GameObject.h>
#include <mye/core/Component.h>

#include <functional>
#include <vector>

#define MYE_LUA_NO_WRAP 0
#define MYA_LUA_BOOST_WRAP 1
#define MYA_LUA_STD_WRAP 2

#define MYE_LUA_WRAP_OPT MYA_LUA_NO_WRAP

#if MYE_LUA_WRAP_OPT == MYA_LUA_BOOST_WRAP

#define MYE_LUA_COMPONENT_WRAP(__Component) boost::ref(__Component)
#define MYE_LUA_COMPONENT_UNWRAP(__Component) (__Component).get()
#define MYE_LUA_COMPONENT_WRAP_TYPE(__Type) boost::reference_wrapper<__Type*>

#elif MYE_LUA_WRAP_OPT == MYA_LUA_STD_WRAP

#define MYE_LUA_COMPONENT_WRAP(__Component) std::ref(__Component)
#define MYE_LUA_COMPONENT_UNWRAP(__Component) (__Component).get()
#define MYE_LUA_COMPONENT_WRAP_TYPE(__Type) std::reference_wrapper<__Type*>

#else

#define MYE_LUA_COMPONENT_WRAP(__Component) (__Component)
#define MYE_LUA_COMPONENT_UNWRAP(__Component) (__Component)
#define MYE_LUA_COMPONENT_WRAP_TYPE(__Type) __Type*
#define MYE_LUA_COMPONENT_UNWRAP_CAST(__Component, __Type) static_cast<__Type*>(__Component)

#endif

namespace mye
{

	namespace lua
	{

		void BindGameObjectHandle(lua_State *L);

		std::string __goh_getname(const mye::core::GameObjectHandle &hObj);

		luabind::object __goh_getcomponent(const mye::core::GameObjectHandle &hObj,
										   const std::string &name);

		void __goh_addcomponent(const mye::core::GameObjectHandle &hObj,
								const mye::core::Component &component);

		void __goh_destroy(const mye::core::GameObjectHandle &hObj);
		bool __goh_exists(const mye::core::GameObjectHandle &hObj);

		std::string __goh_tostring(const mye::core::GameObjectHandle &hObj);

	}

}