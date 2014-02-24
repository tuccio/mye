#pragma once

#include <mye/core/ScriptModule.h>
#include <mye/core/String.h>

#include <lua.hpp>

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

			mye::core::String GetLastError(void) const;

			LuaScript LoadBehaviour(const mye::core::String &filename);
			LuaScript LoadProcedure(const mye::core::String &filename);

			bool RunFile(const mye::core::String &file);
			bool RunString(const mye::core::String &code);

		private:

			void OpenAllLibraries(void);

			lua_State *m_L;
			mye::core::String m_lastError;

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

			bool RunFile(const mye::core::String &file);
			bool RunString(const mye::core::String &code);

			mye::core::String GetLastError(void) const;

			bool Init(void);
			void ShutDown(void);

			mye::core::Script<mye::lua::LuaScriptCaller> LoadClass(const mye::core::String &filename);
			mye::core::Script<mye::lua::LuaScriptCaller> LoadProcedure(const mye::core::String &filename);

		private:

			void OpenAllLibraries(void);

			lua_State *_L;
			mye::core::String _lastError;

		};

	}

}

*/