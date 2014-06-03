#pragma once

#include <mye/core/ResourceManager.h>
#include <mye/core/ScriptModule.h>
#include <mye/core/String.h>

#include "BehaviourScript.h"
#include "ProcedureScript.h"
#include "ScriptResourceLoader.h"
#include "Script.h"

#include <lua.hpp>

namespace mye
{

	namespace lua
	{

		class LuaScript;

		class LuaModule :
			public mye::core::ScriptModule,
			public mye::core::ResourceManager
		{

		public:

			LuaModule(const mye::core::String &folder);
			~LuaModule(void);

			bool Init(void);
			void ShutDown(void);

			void Preupdate(mye::core::FloatSeconds dt);

			lua_State* GetLuaState(void);

			const mye::core::String& GetScriptDirectory(void) const;
			mye::core::String GetLastError(void) const;

			BehaviourScriptPointer      LoadBehaviour(const mye::core::String &name);
			ProcedureScriptPointer      LoadProcedure(const mye::core::String &name);
			ScriptResourceLoaderPointer LoadScriptResourceLoader(const mye::core::String &name);

			bool RunFile(const mye::core::String &file);
			bool RunString(const mye::core::String &code);

		protected:

			Script* CreateImpl(const mye::core::String &name,
				mye::core::ManualResourceLoader *manual,
				const mye::core::Parameters &params);

			void FreeImpl(mye::core::Resource* resource);

		private:

			void OpenAllLibraries(void);

			lua_State *m_lua;
			mye::core::String m_lastError;
			mye::core::String m_scriptDirectory;

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