#include <mye/core/GameObject.h>
#include <mye/core/Game.h>

#include <mye/core/Components.h>

#include <Eigen/Eigen>

#include <iostream>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "Types.h"
#include "Utils.h"

#define GAMEOBJECT_OVERLOADS "GameObjectOverloads"
#define GAMEOBJECT_CLASSES "GameObjectClasses"
#define GAMEOBJECT_TYPE "__gotype"
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

		object __gameobject_getclassmeta(lua_State *L,
										 const GameObjectHandle &hObj)
		{

			GameObject *go = Game::GetGameObject(hObj);

			if (go)
			{

				// Check if "field" is a class method

				std::string gotype = go->GetType();

				if (!gotype.empty())
				{

					return RegistryEntry(L)[GAMEOBJECT_CLASSES][gotype];

				}

			}

			return object();

		}

		object __gameobject_getclassfield(lua_State *L,
										  const GameObjectHandle &hObj,
										  const std::string &field)
		{

			object cMeta = __gameobject_getclassmeta(L, hObj);

			if (type(cMeta) == LUA_TTABLE)
			{
				return cMeta[field];
			}

			return object();

		}

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

			object fieldobj = __gameobject_getclassfield(L, hObj, field);

			if (type(fieldobj) == LUA_TFUNCTION)
			{
				return fieldobj;
			}

			if (go)
			{

				// Check if field is a variable

				VariableComponent *vc = go->GetComponent<VariableComponent>();

				if (vc && vc->Contains(field))
				{

					Variable &v = (*vc)[field];

					std::type_index type = v.GetType();

#define __TYPECHECK_BEGIN(__Type) if (type == typeid(__Type)) { return object(L, v.Get<__Type>()); }
#define __TYPECHECK(__Type) else __TYPECHECK_BEGIN(__Type)
#define __TYPECHECK_END() else { return object(); }

					__TYPECHECK_BEGIN(float)
					__TYPECHECK(int)
					__TYPECHECK(bool)
					__TYPECHECK(std::string)
					__TYPECHECK(Eigen::Vector3f)
					__TYPECHECK_END()
					
				}

			}

			return object();

		}

		void __gameobject_newindex(
			lua_State *L,
			const GameObjectHandle &hObj,
			const std::string &field,
			const object &value)
		{

			// TODO: Add Update, transform, ...
			// TODO: Add variable component handling

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

				case LUA_TNUMBER:

					vc->Add(field).Set<float>(object_cast<float>(value));
					break;

				case LUA_TBOOLEAN:

					vc->Add(field).Set<bool>(object_cast<bool>(value));
					break;

				case LUA_TSTRING:

					vc->Add(field).Set<std::string>(object_cast<std::string>(value));
					break;

				case LUA_TUSERDATA:

					{
						
						auto opvec3 = object_cast_nothrow<Eigen::Vector3f>(value);

						if (opvec3)
						{
							vc->Add(field).Set<Eigen::Vector3f>(*opvec3);
						}
						
					}
					
					break;

				default:
					break;

				}

			}

		}

		void __gameobject_newindex(
			const object &ud,
			const std::string &field,
			const object &value)
		{
			lua_State *L = ud.interpreter();
			GameObjectHandle hObj = object_cast<GameObjectHandle>(ud);
			__gameobject_newindex(L, hObj, field, value);
		}

		/* GameObject static */

		bool __gameobject_static_registerclass(const object &sTable,
			const std::string &name,
			const object &cMeta)
		{

			if (type(cMeta) == LUA_TTABLE &&
				type(cMeta["__init"]) == LUA_TFUNCTION)
			{
				RegistryEntry(sTable.interpreter())[GAMEOBJECT_CLASSES][name] = cMeta;
				return true;
			}

			return false;

		}

		void __gameobject_static_unregisterclass(const object &sTable,
			const std::string &name)
		{
			RegistryEntry(sTable.interpreter())[GAMEOBJECT_CLASSES] = object();
		}

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

			GameObjectHandle hObj = Game::CreateGameObject(classname);

			object ctor = __gameobject_getclassfield(sTable.interpreter(),
													 hObj,
													 "__init");

			if (ctor)
			{

				object o = call_function<object>(ctor);

				for (iterator it(o), end; it != end; it++)
				{
					
					static const std::string &prfx = "__";
					std::string field = object_cast<std::string>(it.key());

					auto checkprefix = std::mismatch(prfx.begin(), prfx.end(), field.begin());										

					if (checkprefix.first != prfx.end() &&
						type(*it) != LUA_TFUNCTION)
					{
						__gameobject_newindex(sTable.interpreter(), hObj, field, *it);
					}

				}

			}

			return hObj;
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
				def("new", (GameObjectHandle (*) (void)) &Game::CreateGameObject),
				def("new", (GameObjectHandle (*) (const std::string&)) &Game::CreateGameObject),
				def("new", (GameObjectHandle (*) (const object&, const std::string&)) &__gameobject_static_new),
				def("new", (GameObjectHandle (*) (const object&)) &__gameobject_static_new)
			];

			sAuxTable["new"] = OVERLOAD_SENTINEL;
			sAuxTable["RegisterClass"] = make_function(L, &__gameobject_static_registerclass);
			sAuxTable["UnregisterClass"] = make_function(L, &__gameobject_static_unregisterclass);

			setmetatable(sAuxTable, sAuxMeta);

			RegistryEntry(L)[MYE_LUA_GAMEOBJECT_STATIC_AUX] = sAuxTable;
			RegistryEntry(L)[GAMEOBJECT_CLASSES] = newtable(L);

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
			mt["__newindex"] = make_function(L,
											 (void (*) (const object &, 
														const std::string &,
														const object &)) &__gameobject_newindex);

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