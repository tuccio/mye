#pragma once

#include <mye/core/Game.h>
#include <mye/core/ScriptModule.h>

#include <lua.hpp>
#include <string>

namespace mye
{

	namespace lua
	{

		class LuaScript;

		class LuaModule :
			public mye::core::ScriptModule
		{

		public:

			LuaModule(void);
			~LuaModule(void);

			bool Init(void);
			void ShutDown(void);

			lua_State* GetLuaState(void);

			std::string GetLastError(void) const;

			LuaScript LoadBehaviour(const std::string &filename);
			LuaScript LoadProcedure(const std::string &filename);

			bool RunFile(const std::string &file);
			bool RunString(const std::string &code);

		private:

			void OpenAllLibraries(void);

			lua_State *m_L;
			std::string m_lastError;

		};

	}

}

/*
namespace mye
{

	namespace lua
	{
		struct LuaScriptCaller;
		typedef mye::core::ScriptModule<LuaScriptCaller> LuaModule;
	}

}


namespace mye
{

	namespace core
	{

		template <>
		class ScriptModule<mye::lua::LuaScriptCaller> :
			public IScriptModule
		{

		public:

			ScriptModule(void);
			~ScriptModule(void);

			lua_State* GetLuaState(void);

			bool RunFile(const std::string &file);
			bool RunString(const std::string &code);

			std::string GetLastError(void) const;

			bool Init(void);
			void ShutDown(void);

			mye::core::Script<mye::lua::LuaScriptCaller> LoadClass(const std::string &filename);
			mye::core::Script<mye::lua::LuaScriptCaller> LoadProcedure(const std::string &filename);

		private:

			void OpenAllLibraries(void);

			lua_State *_L;
			std::string _lastError;

		};

	}

}

*/