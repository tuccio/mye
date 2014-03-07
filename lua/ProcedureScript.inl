#include <mye/core/Utils.h>

namespace mye
{

	namespace lua
	{

		void ProcedureScript::Run(void)
		{

			int top = lua_gettop(m_lua);

			lua_rawgeti(m_lua, LUA_REGISTRYINDEX, m_registryReference);

			if (lua_pcall(m_lua, 0, 0, 0))
			{

				mye::core::String errorMessage = lua_tostring(m_lua, -1);
				mye::core::RuntimeError(errorMessage);

				luaL_error(m_lua, errorMessage.CString());

			}
			
			lua_settop(m_lua, top);

		}

	}

}