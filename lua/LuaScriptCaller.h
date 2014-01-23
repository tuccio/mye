/*
#pragma once

#include <mye/core/Script.h>
#include <mye/core/ScriptModule.h>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <string>

namespace mye
{

	namespace lua
	{

		struct LuaScriptCaller
		{

			static mye::core::Script<LuaScriptCaller> LoadClass(
				mye::core::ScriptModule<LuaScriptCaller> &module,
				const std::string &filename);

			static mye::core::Script<LuaScriptCaller> LoadProcedure(
				mye::core::ScriptModule<LuaScriptCaller> &module,
				const std::string &filename);

			static bool Run(mye::core::ScriptModule<LuaScriptCaller> &module,
				const mye::core::Script<LuaScriptCaller> &script);

			template <typename R>
			static R Call(
				mye::core::ScriptModule<LuaScriptCaller> &module,
				const mye::core::Script<LuaScriptCaller> &script,
				const std::string &f);

		};

	}

}

#include "LuaScriptCaller.inl"*/