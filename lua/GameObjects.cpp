#include <mye/core/GameObject.h>
#include <mye/core/Game.h>

#include <iostream>

#include <lua.hpp>

#include "Types.h"
#include "Utils.h"

using namespace mye::core;

namespace mye
{

	namespace lua
	{

		const char *hello(void)
		{
			return "hello";
		}

		int __gameobject_new(lua_State *L);

		int __gameobject_create(lua_State *L);
		int __gameobject_destroy(lua_State *L);
		int __gameobject_exists(lua_State *L);

		int __gameobject_newindex(lua_State *L);
		int __gameobject_proxy(lua_State *L);

		void RegisterGameObjects(lua_State *L)
		{

			// Create a proxy table

			lua_newtable(L);

			// Create a metatable

			lua_newtable(L);

			lua_pushcfunction(L, __gameobject_proxy);
			lua_setfield(L, -2, "__index");

			lua_pushcfunction(L, __mye_donothing);
			lua_setfield(L, -2, "__newindex");

			lua_pushnil(L);
			lua_setfield(L, -2, "__metatable");

			lua_setmetatable(L, -2);

			lua_setglobal(L, MYE_LUA_GAMEOBJECT);

		}

		int __gameobject_new(lua_State *L)
		{

			// Create the table for the instance

			lua_newuserdata(L, sizeof(GameObjectHandle));

			// Create the metatable			

			if (luaL_newmetatable(L, MYE_LUA_GAMEOBJECT))
			{
				lua_pushcfunction(L, __gameobject_newindex);
				lua_setfield(L, -2, "__newindex");
			}

			lua_setmetatable(L, -2);

			return 1;

		}

		int __gameobject_newindex(lua_State *L)
		{

			int nargs = lua_gettop(L);

			if (nargs != 3)
			{
				luaL_error(L, "GameObject: Assignment argument count error");
				return 0;
			}

			void *d = luaL_checkudata(L, 1, MYE_LUA_GAMEOBJECT);				

			if (!d)
			{
				luaL_error(L, "GameObject: Call error");
			}

			GameObjectHandle hObj;

			memcpy(&hObj,
				d,
				sizeof(GameObjectHandle));

			const char *field = lua_tostring(L, 2);

			if (field[0] != '_')
			{

				int objectType = lua_type(L, 3);

				switch (objectType)
				{

				case LUA_TSTRING:
					std::cout << "string assignment" << std::endl;
					break;

				case LUA_TTABLE:
					std::cout << "table assignment" << std::endl;
					break;

				case LUA_TNUMBER:
					std::cout << "number assignment" << std::endl;
					break;

				case LUA_TNIL:
					std::cout << "nil assignment" << std::endl;
					break;

				default:						
					luaL_error(L, "GameObject: Assignment type error");
					break;

				}

				return 0;

			}
			else
			{
				luaL_error(L, "GameObject: Identifiers prefixed by underscores are reserved");
				return 0;
			}

		}

		int __gameobject_proxy(lua_State *L)
		{

			if (strcmp("new", lua_tostring(L, 2)) == 0)
			{
				lua_pushcfunction(L, __gameobject_new);
				return 1;
			}

			return 0;

		}

	}

}