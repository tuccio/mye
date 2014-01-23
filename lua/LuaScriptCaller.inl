
/*
namespace mye
{

	namespace lua
	{

		template <typename R>
		R LuaScriptCaller::Call(mye::core::ScriptModule<LuaScriptCaller> &module,
			const mye::core::Script<LuaScriptCaller> &script,
			const std::string &f)
		{

			using namespace luabind;

			lua_State *L = module.GetLuaState();			
			int ref = *script.GetReference();

			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			object table(from_stack(L, -1));

			return call_function<R>(table[f]);

		}

	}

}*/