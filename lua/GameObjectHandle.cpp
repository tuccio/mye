#include "GameObjectHandle.h"

#include "Alignment.h"
#include "Converters.h"
#include "MetaMethodsOverload.h"
#include "Types.h"
#include "Utils.h"

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

				class_<GameObjectHandle>(MYE_LUA_GAMEOBJECTHANDLE).

					def("GetName", &__goh_getname).

					def("GetComponent", &__goh_getcomponent).
					def("AddComponent", &__goh_addcomponent).

					def("Exists", &__goh_exists).
					def("Destroy", &__goh_destroy).

					def("__tostring", &__goh_tostring).

					property("name", &__goh_getname)

			];

			// Overload the __newindex and __index methods to access components as properties

			OverloadLuabindMetamethod(L, typeid(GameObjectHandle), "__newindex", &NewIndexOverload);
			OverloadLuabindMetamethod(L, typeid(GameObjectHandle), "__index", &IndexOverload);

		}

		mye::core::String __goh_getname(const GameObjectHandle &hObj)
		{
			return LPGAMEOBJECT(hObj)->GetName();
		}

		object __goh_getcomponent(const GameObjectHandle &hObj, const mye::core::String &name)
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
			Game::GetSingleton().GetGameObjectsModule()->PostDestroy(hObj);
		}

		bool __goh_exists(const mye::core::GameObjectHandle &hObj)
		{
			return LPGAMEOBJECT(hObj) != nullptr;
		}

		mye::core::String __goh_tostring(const GameObjectHandle &hObj)
		{

			mye::core::String result;
			GameObject *go = LPGAMEOBJECT(hObj);

			if (go)
			{

				mye::core::String name = go->GetName();

				if (name.IsEmpty())
				{
					result = mye::core::String("<Unnamed object [") +
						mye::core::ToString(hObj.id) +
						mye::core::String(", ") +
						mye::core::ToString(hObj.allocation) +
						mye::core::String("]>");
				}
				else
				{
					result = mye::core::String("<" + name + " [") +
						mye::core::ToString(hObj.id) +
						mye::core::String(", ") +
						mye::core::ToString(hObj.allocation) +
						mye::core::String("]>");
				}

			}
			else
			{
				result = mye::core::String("<Invalid object>");
			}

			return result;

		}

	}

}