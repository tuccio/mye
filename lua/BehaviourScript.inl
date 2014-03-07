#include "Debug.h"

namespace mye
{

	namespace lua
	{

		template <typename R>
		R BehaviourScript::Call(const mye::core::String &f) const
		{

			using namespace luabind;

			lua_rawgeti(m_lua, LUA_REGISTRYINDEX, m_registryReference);
			object table(from_stack(m_lua, -1));
			lua_remove(m_lua, -1);

			return static_cast<R>(call_function<R>(table[f]));

		}

		template <typename R, typename T1>
		R BehaviourScript::Call(const mye::core::String &f, T1 arg1) const
		{

			using namespace luabind;

			lua_rawgeti(m_lua, LUA_REGISTRYINDEX, m_registryReference);
			object table(from_stack(m_lua, -1));
			lua_remove(m_lua, -1);

			return static_cast<R>(call_function<R, T1>(table[f], arg1));

		}

		template <typename R, typename T1, typename T2>
		R BehaviourScript::Call(const mye::core::String &f, T1 arg1, T2 arg2) const
		{

			using namespace luabind;

			lua_rawgeti(m_lua, LUA_REGISTRYINDEX, m_registryReference);
			object table(from_stack(m_lua, -1));
			lua_remove(m_lua, -1);

			return static_cast<R>(call_function<R, T1, T2>(table[f], arg1, arg2));

		}

	}

}