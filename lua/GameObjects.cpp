#include <mye/core/GameObject.h>
#include <mye/core/Game.h>

#include <mye/core/Components.h>

#include <iostream>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "Types.h"
#include "Utils.h"

#define GAMEOBJECT_OVERLOADS "GameObjectOverloads"
#define GAMEOBJECT_CLASSES "GameObjectClasses"
#define OVERLOAD_SENTINEL "__overload"

using namespace mye::core;
using namespace mye::lua;
using namespace luabind;

namespace luabind
{

	template <>
	struct default_converter<GameObjectHandle> :
		native_converter_base<GameObjectHandle>
	{

		static int compute_score(lua_State *L, int index)
		{
			return GetMYEType(L, index) == MYE_LUA_GAMEOBJECT;
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
			object udmt = mye::lua::RegistryEntry(L)[MYE_LUA_GAMEOBJECT];

			setmetatable(ud, udmt);

			SetMYEType(ud, MYE_LUA_GAMEOBJECT);

			memcpy(d, &hObj, sizeof(GameObjectHandle));

		}

	};

	template <>
	struct default_converter<const GameObjectHandle&> :
		default_converter<GameObjectHandle>
	{

	};

}

/* GameObject static */

namespace mye
{

	namespace lua
	{

		/* GameObject */

		object __gameobject_index(
			const object &ud,
			const std::string &field)
		{

			lua_State *L = ud.interpreter();

			object o = RegistryEntry(L)[MYE_LUA_GAMEOBJECT_AUX][field];

			if (type(o) == LUA_TFUNCTION)
			{
				return o;
			}

			// TODO: Add Update, transform, ...
			// TODO: Add variable component handling

			GameObjectHandle hObj = object_cast<GameObjectHandle>(ud);

			GameObject *go = Game::GetGameObject(hObj);

			if (go)
			{

				VariableComponent *vc = go->GetComponent<VariableComponent>();

				if (vc && vc->Contains(field))
				{

					Variable &v = (*vc)[field];

					std::type_index type = v.GetType();

					if (type == typeid(float))
					{
						return object(L, v.Get<float>());
					}
					else if (type == typeid(object))
					{
						return v.Get<object>();
					}
					
				}

			}

			return object();

		}

		void __gameobject_newindex(
			const object &ud,
			const std::string &field,
			const object &value)
		{

			// TODO: Add Update, transform, ...
			// TODO: Add variable component handling

			lua_State *L = ud.interpreter();
			GameObjectHandle hObj = object_cast<GameObjectHandle>(ud);

			GameObject *go = Game::GetGameObject(hObj);

			if (go)
			{

				VariableComponent *vc = go->GetComponent<VariableComponent>();

				if (!vc)
				{
					go->AddComponent(VariableComponent());
					vc = go->GetComponent<VariableComponent>();
				}

				Variable &v = vc->Add(field);

				switch (type(value))
				{

				case LUA_TFUNCTION:

					value.push(L);
					v.Set<int>(luaL_ref(L, LUA_REGISTRYINDEX));
					lua_pop(L, 1);
					
					break;

				default:
					break;

				}

			}

		}

		/* GameObject static */

		object __gameobject_static_index(
			const object &table,
			const std::string &field)
		{

			lua_State *L = table.interpreter();

			object o = RegistryEntry(L)[MYE_LUA_GAMEOBJECT_STATIC_AUX][field];

			if (type(o) == LUA_TSTRING &&
				object_cast<std::string>(o) == OVERLOAD_SENTINEL)
			{

				return globals(L)[GAMEOBJECT_OVERLOADS][field];

			}

			return o;

		}

		GameObjectHandle __gameobject_static_new(const object &sTable)
		{
			return Game::CreateGameObject();
		}

		GameObjectHandle __gameobject_static_new(const object &sTable,
			const std::string &classname)
		{

			GameObjectHandle hObj = Game::CreateGameObject();

			lua_State *L = sTable.interpreter();

			object cTable = globals(L)[classname];

			if (cTable && type(cTable) == LUA_TTABLE)
			{

				object ctor;

				default_converter<GameObjectHandle> conv;
				conv.to(L, hObj);
				object ud = object(from_stack(L, -1));

				for (iterator it(cTable), end; it != end; it++)
				{

					std::string key = object_cast<std::string>(it.key());
					std::string prfx = "__";

					auto res = std::mismatch(prfx.begin(), prfx.end(), key.begin());

					if (res.first != prfx.end())
					{
						std::cout << "Add " << key << std::endl;
						__gameobject_newindex(ud, key, *it);
					}
					else if (key == "__init" && type(*it) == LUA_TFUNCTION)
					{
						ctor = *it;
					}
					else
					{
						std::cout << "Discard " << key << std::endl;
					}

				}

				if (ctor)
				{
					std::cout << "Got constructor" << std::endl;
				}

			}

			return hObj;

		}

		void __gameobject_static_registerclass(const std::string &name,
			const object &c)
		{

			lua_State *L = c.interpreter();
			RegistryEntry(L)[GAMEOBJECT_CLASSES][name] = c;

		}

		

		/* Register stuff */

		void RegisterGameObjectStaticTable(lua_State *L)
		{

			object sTable = newtable(L);
			object sMeta = newtable(L);

			sMeta["__metatable"] = false;

			sMeta["__newindex"] = RegistryEntry(L)[MYE_LUA_NOOP];
			sMeta["__index"] = make_function(L, &__gameobject_static_index);

			setmetatable(sTable, sMeta);

			globals(L)[MYE_LUA_GAMEOBJECT] = sTable;

			object sAuxTable = newtable(L);
			object sAuxMeta = newtable(L);

			sAuxMeta["__metatable"] = false;
			sAuxMeta["__newindex"] = RegistryEntry(L)[MYE_LUA_NOOP];

			module(L, GAMEOBJECT_OVERLOADS)
			[
				def("new", &Game::CreateGameObject),
				def("new", (GameObjectHandle (*) (const object&, const std::string&)) &__gameobject_static_new),
				def("new", (GameObjectHandle (*) (const object&)) &__gameobject_static_new)
			];



			sAuxTable["new"] = OVERLOAD_SENTINEL;

			setmetatable(sAuxTable, sAuxMeta);

			RegistryEntry(L)[MYE_LUA_GAMEOBJECT_STATIC_AUX] = sAuxTable;

		}

		void RegisterGameObjectTable(lua_State *L)
		{

			object sTable = newtable(L);
			object sMeta = newtable(L);

			sMeta["__metatable"] = false;
			sMeta["__newindex"] = RegistryEntry(L)[MYE_LUA_NOOP];

			sTable["Exists"] = make_function(L, &Game::IsGameObject);
			sTable["Destroy"] = make_function(L, &Game::DestroyGameObject);

			setmetatable(sTable, sMeta);

			RegistryEntry(L)[MYE_LUA_GAMEOBJECT_AUX] = sTable;

		}

		void RegisterGameObjectHandleTable(lua_State *L)
		{

			object mt = newtable(L);

			mt["__index"] = make_function(L, &__gameobject_index);
			mt["__newindex"] = make_function(L, &__gameobject_newindex);

			RegistryEntry(L)[MYE_LUA_GAMEOBJECT] = mt;

		}

		void RegisterGameObjects(lua_State *L)
		{
			RegisterGameObjectStaticTable(L);
			RegisterGameObjectTable(L);
			RegisterGameObjectHandleTable(L);
		}

	}

}