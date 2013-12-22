#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <string>

#define MYE_LUA_REGKEY "mye"
#define MYE_LUA_PUSHNIL "pushnil"
#define MYE_LUA_NOOP "noop"

namespace mye
{

	namespace lua
	{

		void RegisterUtils(lua_State *L);

		template <typename F>
		void RegisterFunction(const luabind::object &table,
			const std::string &name,
			F f);

		luabind::object RegistryEntry(lua_State *L);

		void* UserdataMetatableCheck(lua_State *L, int index, const char *type);

		std::string GetMYEType(const luabind::object &o);
		std::string GetMYEType(lua_State *L, int index);

		void SetMYEType(const luabind::object &o, const std::string &type);
		void SetMYEType(lua_State *L, int index);

		int __mye_pushnil(lua_State *L);
		int __mye_noop(lua_State *L);


	}

}

/* Template implementation */

namespace mye
{

	namespace lua
	{

		template <typename F>
		void RegisterFunction(const luabind::object &table,
			const std::string &name,
			F f)
		{

			using namespace luabind;

			lua_State *L = table.interpreter();

			registry(L)[]

		}
	}

}