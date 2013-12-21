#include <lua.hpp>
#include <luabind/luabind.hpp>

#define MYE_LUA_PUSHNIL "mye_pushnil"
#define MYE_LUA_NOOP "mye_noop"

namespace mye
{

	namespace lua
	{

		int __mye_pushnil(lua_State *L);
		int __mye_noop(lua_State *L);

		void RegisterUtils(lua_State *L);

	}

}