#include <luabind/luabind.hpp>

namespace mye
{

	namespace lua
	{

		/*template <typename R>
		R Script<mye::lua::LuaScriptCaller>::Call(const std::string &f) const
		{
			return mye::lua::LuaScriptCaller::Call<R>(m_module, *this, f);
		}*/

		template <typename R>
		R LuaScript::Call(const std::string &f) const
		{

			using namespace luabind;

			lua_State *L = m_module.GetLuaState();			
			int ref = *GetReference();

			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			object table(from_stack(L, -1));
			lua_remove(L, -1);

			return static_cast<R>(call_function<R>(table[f]));

		}

		template <typename R, typename T1>
		R LuaScript::Call(const std::string &f, T1 arg1) const
		{

			using namespace luabind;

			lua_State *L = m_module.GetLuaState();			
			int ref = *GetReference();

			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			object table(from_stack(L, -1));
			lua_remove(L, -1);

			return static_cast<R>(call_function<R, T1>(table[f], arg1));

		}

	}

}