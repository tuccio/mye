#include "GameObjectHandle.h"

#include "Types.h"
#include "Util.h"
#include "MetaMethodsOverload.h"

#include <mye/core/Components.h>
#include <mye/core/Game.h>

#include <mye/math/Math.h>
#include <luabind/luabind.hpp>

#define LPGAMEOBJECT(__hObj) (Game::GetSingleton().GetGameObjectsModule()->Get(__hObj))

using namespace mye::core;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		void BindGameObjectHandle(lua_State *L)
		{

			module(L)
			[
				class_<Component>("Component")
			];

			module(L)
			[

				class_<GameObjectHandle>(MYE_LUA_GAMEOBJECTHANDLE).

					def("GetName", &__goh_getname).

					def("GetComponent", &__goh_getcomponent).
					def("AddComponent", &__goh_addcomponent).

					def("Exists", &__goh_exists).
					def("Destroy", &__goh_destroy).

					def("__tostring", &__goh_tostring)

			];

			// Overload the __newindex and __index methods to access components as properties

			OverloadLuabindMetamethod(L, typeid(GameObjectHandle), "__newindex", &NewIndexOverload);
			OverloadLuabindMetamethod(L, typeid(GameObjectHandle), "__index", &IndexOverload);

		}

		std::string __goh_getname(const GameObjectHandle &hObj)
		{
			return LPGAMEOBJECT(hObj)->GetName();
		}

		object __goh_getcomponent(const GameObjectHandle &hObj, const std::string &name)
		{
			Component *component = LPGAMEOBJECT(hObj)->GetComponent(name);
			return object(LPLUASTATE, MYE_LUA_COMPONENT_WRAP(component));
		}

		void __goh_addcomponent(const GameObjectHandle &hObj,
			const Component &component)
		{
			LPGAMEOBJECT(hObj)->AddComponent(component);
		}

		void __goh_destroy(const mye::core::GameObjectHandle &hObj)
		{
			Game::GetSingleton().GetGameObjectsModule()->Destroy(hObj);
		}

		bool __goh_exists(const mye::core::GameObjectHandle &hObj)
		{
			return LPGAMEOBJECT(hObj) != nullptr;
		}

		std::string __goh_tostring(const GameObjectHandle &hObj)
		{

			std::string name = LPGAMEOBJECT(hObj)->GetName();

			if (name.empty())
			{
				return std::string("<Unnamed object [") +
					std::to_string(hObj.id) +
					std::string(", ") +
					std::to_string(hObj.allocation) +
					std::string("]>");
			}

			return name;

		}

	}

}