//#include "MetaMethodsOverload.h"
//#include "Math.h"
//#include "Utils.h"
//#include "GameObjectHandle.h"
//#include "Converters.h"
//#include <mye/core/Components.h>
//#include <mye/core/Game.h>
//#include <mye/core/GameObjectsModule.h>
//#include <mye/core/GameObject.h>
//#include <mye/math/Math.h>
//
//#include <lua.hpp>
//#include <luabind/luabind.hpp>
//#include <luabind/copy_policy.hpp>
//#include <luabind/shared_ptr_converter.hpp>
//#include <luabind/typecheck.hpp>
//
//#include "Debug.h"
//
//#include <functional>
//#include <vector>
//#include <map>
//#include <unordered_map>
//
//using namespace luabind;
//using namespace mye::core;
//using namespace mye::math;
//
//namespace mye
//{
//
//	namespace lua
//	{
//
//		/*static std::unordered_map<std::type_index, std::pair<TypeTest, lua_CFunction>> __indexOverloads;
//		static std::unordered_map<std::type_index, std::pair<TypeTest, lua_CFunction>> __newindexOverloads;
//
//		void RegisterIndexOverload(const std::type_info &typeInfo, TypeTest t, lua_CFunction f)
//		{
//			__indexOverloads[typeInfo] = (std::pair<TypeTest, lua_CFunction>(t, f));
//		}
//
//		void RegisterNewIndexOverload(const std::type_info &typeInfo, TypeTest t, lua_CFunction f)
//		{
//			__newindexOverloads[typeInfo] = (std::pair<TypeTest, lua_CFunction>(t, f));
//		}*/
//
//		static std::vector<std::pair<TypeTest, lua_CFunction>> __indexOverloads;
//		static std::vector<std::pair<TypeTest, lua_CFunction>> __newindexOverloads;
//
//		void RegisterIndexOverload(TypeTest t, lua_CFunction f)
//		{
//			__indexOverloads.push_back(std::pair<TypeTest, lua_CFunction>(t, f));
//		}
//
//		void RegisterNewIndexOverload(TypeTest t, lua_CFunction f)
//		{
//			__newindexOverloads.push_back(std::pair<TypeTest, lua_CFunction>(t, f));
//		}
//
//		int IndexOverload(lua_State *L)
//		{
//
//			int top = lua_gettop(L);
//
//			int returnValues = 0;
//
//			lua_pushvalue(L, lua_upvalueindex(1));
//			lua_pushvalue(L, 1);
//			lua_pushvalue(L, 2);
//
//			lua_call(L, 2, 1);
//
//			if (lua_isnil(L, 3))
//			{
//
//				lua_pop(L, 1);
//
//				const char *field = lua_tostring(L, 2);
//
//				if (strncmp(field, "__", 2)) // Ignore metamethods like __finalize, __init, ...
//				{
//
//					object obj(from_stack(L, 1));					
//
//					bool done = false;
//
//					for (auto overload : __indexOverloads)
//					{
//
//						if (overload.first(obj))
//						{
//							returnValues = overload.second(L);
//							done = true;
//							break;
//						}
//
//					}
//
//					if (!done)
//					{
//						lua_pushnil(L);
//						returnValues = 1;
//					}
//
//				}
//
//			}
//			else
//			{
//				returnValues = 1;
//			}
//
//			lua_settop(L, top + returnValues);
//
//			return returnValues;
//
//		}
//
//		int NewIndexOverload(lua_State *L)
//		{
//
//			LuaStackCleaner stackCleaner(L);
//
//			object obj(from_stack(L, 1));
//			bool done = false;
//
//			for (auto overload : __newindexOverloads)
//			{
//
//				if (overload.first(obj))
//				{
//					overload.second(L);
//					done = true;
//					break;
//				}
//
//			}
//
//			if (!done)
//			{
//
//				lua_pushvalue(L, lua_upvalueindex(1));
//				lua_pushvalue(L, 1);
//				lua_pushvalue(L, 2);
//				lua_pushvalue(L, 3);
//
//				lua_call(L, 3, 0);
//
//
//			}
//
//			return 0;
//
//		}
//
//		bool OverloadLuabindMetamethod(lua_State *L,
//			const char *metamethod,
//			lua_CFunction f)
//		{
//
//			using namespace luabind;
//
//			auto reg = detail::class_registry::get_registry(L);
//
//			if (reg)
//			{
//
//				int ref = reg->cpp_instance();
//
//				// Stack : -
//
//				lua_pushinteger(L, ref);
//				lua_gettable(L, LUA_REGISTRYINDEX);
//
//				// Stack : metatable
//
//				lua_pushstring(L, metamethod);
//
//				lua_pushstring(L, metamethod);
//				lua_rawget(L, -3);
//
//				// Stack : metatable metamethod function
//
//				lua_pushcclosure(L, f, 1);
//
//				// Stack : metatable metamethod closure
//
//				lua_rawset(L, -3);
//
//				return true;
//
//			}
//
//			return false;
//
//		}
//
//
//
//	}
//
//}