#include <mye/core/GameObject.h>
#include <mye/core/Game.h>

#include <iostream>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "Types.h"
#include "Utils.h"

using namespace mye::core;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		/* GameObject static */

		luabind::object __gameobject_static_index(
			const luabind::object &table,
			const std::string &field)
		{

			if (field == "new")
			{
				lua_State *L = table.interpreter();
				return make_function(L, &Game::CreateGameObject);
			}

			return luabind::object();

		}

		void RegisterGameObjectStaticTable(lua_State *L)
		{

			object sTable = newtable(L);
			object sMeta = newtable(L);

			sMeta["__metatable"] = false;

			sMeta["__newindex"] = registry(L)[MYE_LUA_NOOP];
			sMeta["__index"] = make_function(L, &__gameobject_static_index);

			setmetatable(sTable, sMeta);

			globals(L)[MYE_LUA_GAMEOBJECT] = sTable;

		}

		/* GameObject */

		static std::map<std::string, luabind::object> __gameobject_methods;

		luabind::object __gameobject_index(
			const luabind::object &table,
			const std::string &field)
		{

			if (field == "Exists")
			{
				lua_State *L = table.interpreter();
				return make_function(L, &Game::IsGameObject);
			}

			if (field == "Destroy")
			{
				lua_State *L = table.interpreter();
				return make_function(L, &Game::DestroyGameObject);
			}

			// TODO: Add Update, transform, ...
			// TODO: Add variable component handling

			return luabind::object();

		}

		void __gameobject_newindex(
			const luabind::object &table,
			const std::string &field,
			const luabind::object &value)
		{

			// TODO: Add Update, transform, ...
			// TODO: Add variable component handling

		}

		void RegisterGameObjects(lua_State *L)
		{

			RegisterGameObjectStaticTable(L);

		}

	}

}

namespace luabind
{

	template <>
	struct default_converter<GameObjectHandle> :
		native_converter_base<GameObjectHandle>
	{

		static int compute_score(lua_State *L, int index)
		{
			return luaL_checkudata(L, index, MYE_LUA_GAMEOBJECT) != NULL;
		}

		GameObjectHandle from(lua_State *L, int index)
		{

			void *d = lua_touserdata(L, index);

			if (d)
			{
				return * (GameObjectHandle *) d;
			}
			else
			{
				return GameObjectHandle();
			}

		}

		void to(lua_State *L, const GameObjectHandle &hObj)
		{

			void *d = lua_newuserdata(L, sizeof(GameObjectHandle));

			object ud = object(from_stack(L, -1));

			if (luaL_newmetatable(L, MYE_LUA_GAMEOBJECT))
			{


				object mt = object(from_stack(L, -1));

				mt["__index"] = make_function(L, &mye::lua::__gameobject_index);
				mt["__newindex"] = make_function(L, &mye::lua::__gameobject_newindex);

				setmetatable(ud, mt);

			}
			else
			{
				setmetatable(ud, object(from_stack(L, -1)));
			}

			memcpy(d, &hObj, sizeof(GameObjectHandle));

		}

	};

	template <>
	struct default_converter<const GameObjectHandle&> :
		default_converter<GameObjectHandle>
	{

	};

}