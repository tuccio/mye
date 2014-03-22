#include <luabind/luabind.hpp>

namespace mye
{

	namespace lua
	{

		/*template <typename R>
		R Script<mye::lua::LuaScriptCaller>::Call(const mye::core::String &f) const
		{
			return mye::lua::LuaScriptCaller::Call<R>(m_module, *this, f);
		}*/

		int Script::GetRegistryReference(void)
		{
			return m_registryReference;
		}

		lua_State* Script::GetLuaState(void)
		{
			return m_lua;
		}

	}

}