#include "Debug.h"

#include <type_traits>

namespace mye
{

	namespace lua
	{

		enum class CallExceptionError
		{
			UNKNOWN,
			UNDEFINED_SCRIPT,
			UNDEFINED_FUNCTION,
			RUNTIME
		};

		struct CallException
		{

			CallException(CallExceptionError e, const char *msg = nullptr) :
				error(e),
				msg(msg)
			{

			}

			CallExceptionError error;
			mye::core::String msg;

		};

		template <typename R>
		R BehaviourScript::Call(const mye::core::String &f) const
		{

			using namespace luabind;

			if (m_registryReference == LUA_NOREF)
			{
				throw CallException(CallExceptionError::UNDEFINED_SCRIPT);
			}

			lua_rawgeti(m_lua, LUA_REGISTRYINDEX, m_registryReference);

			lua_pushstring(m_lua, f.CString());
			lua_rawget(m_lua, -2);

			int t = lua_type(m_lua, -1);

			if (t != LUA_TFUNCTION)
			{
				throw CallException(CallExceptionError::UNDEFINED_FUNCTION);
			}

			try
			{
				object function(from_stack(m_lua, -1));
				return static_cast<R>(call_function<R>(function));
			}
			catch (error e)
			{
				throw CallException(CallExceptionError::RUNTIME, lua_tostring(m_lua, -1));
			}
			catch (...)
			{
				throw CallException(CallExceptionError::UNKNOWN);
			}

		}

		template <typename R, typename T1>
		R BehaviourScript::Call(const mye::core::String &f, T1 arg1) const
		{

			using namespace luabind;

			if (m_registryReference == LUA_NOREF)
			{
				throw CallException(CallExceptionError::UNDEFINED_SCRIPT);
			}

			lua_rawgeti(m_lua, LUA_REGISTRYINDEX, m_registryReference);

			lua_pushstring(m_lua, f.CString());
			lua_rawget(m_lua, -2);

			int t = lua_type(m_lua, -1);

			if (t != LUA_TFUNCTION)
			{
				lua_pop(m_lua, 2);
				throw CallException(CallExceptionError::UNDEFINED_FUNCTION);
			}

			try
			{

				object function(from_stack(m_lua, -1));
				return static_cast<R>(call_function<R, T1>(function, arg1));

			}
			catch (error e)
			{
				throw CallException(CallExceptionError::RUNTIME, lua_tostring(m_lua, -1));
			}
			catch (...)
			{
				throw CallException(CallExceptionError::UNKNOWN);
			}

		}

		template <typename R, typename T1, typename T2>
		R BehaviourScript::Call(const mye::core::String &f, T1 arg1, T2 arg2) const
		{

			using namespace luabind;

			if (m_registryReference == LUA_NOREF)
			{
				throw CallException(CallExceptionError::UNDEFINED_SCRIPT);
			}

			lua_rawgeti(m_lua, LUA_REGISTRYINDEX, m_registryReference);

			lua_pushstring(m_lua, f.CString());
			lua_rawget(m_lua, -2);

			int t = lua_type(m_lua, -1);

			if (t != LUA_TFUNCTION)
			{
				throw CallException(CallExceptionError::UNDEFINED_FUNCTION);
			}

			try
			{

				object function(from_stack(m_lua, -1));
				return static_cast<R>(call_function<R, T1, T2>(function, arg1, arg2));

			}
			catch (error e)
			{
				throw CallException(CallExceptionError::RUNTIME, lua_tostring(m_lua, -1));
			}
			catch (...)
			{
				throw CallException(CallExceptionError::UNKNOWN);
			}

		}

	}

}