#include <lua.hpp>
#include <typeinfo>

namespace mye
{

	namespace lua
	{

		int IndexOverload(lua_State *L);
		int NewIndexOverload(lua_State *L);

		bool OverloadLuabindMetamethod(lua_State *L,
			const std::type_info &classtype,
			const char *metamethod,
			int (*f) (lua_State*));

	}

}