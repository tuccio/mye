#include <lua.hpp>
#include <typeinfo>

#include <luabind/typecheck.hpp>

namespace mye
{

	namespace lua
	{

		int IndexOverload(lua_State *L);
		int NewIndexOverload(lua_State *L);

		bool OverloadLuabindMetamethod(lua_State *L,
			const char *metamethod,
			lua_CFunction);

		/*bool OverloadLuabindMetamethod(lua_State *L,
			const std::type_info &classtype,
			const char *metamethod,
			int (*f) (lua_State*));*/

		typedef std::function<bool(const luabind::object&)> TypeTest;

		void RegisterIndexOverload(TypeTest t, lua_CFunction f);
		void RegisterNewIndexOverload(TypeTest t, lua_CFunction f);

		template <typename T>
		void RegisterIndexOverload(lua_CFunction f)
		{

			RegisterIndexOverload(TypeTest(&luabind::object_typecheck<T, luabind::object>), f);

		}

		template <typename T>
		void RegisterNewIndexOverload(lua_CFunction f)
		{

			RegisterNewIndexOverload(TypeTest(&luabind::object_typecheck<T, luabind::object>), f);

		}

		/*void RegisterIndexOverload(const std::type_info &typeInfo, TypeTest t, lua_CFunction f);
		void RegisterNewIndexOverload(const std::type_info &typeInfo, TypeTest t, lua_CFunction f);

		template <typename T>
		void RegisterIndexOverload(lua_CFunction f)
		{

			RegisterIndexOverload(typeid(T), TypeTest(&luabind::object_typecheck<T, luabind::object>), f);

		}

		template <typename T>
		void RegisterNewIndexOverload(lua_CFunction f)
		{

			RegisterIndexOverload(typeid(T), RegisterNewIndexOverload(typeid(T)), TypeTest(&luabind::object_typecheck<T, luabind::object>), f);

		}*/

	}

}