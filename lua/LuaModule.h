#pragma once

#include <mye/core/ResourceManager.h>
#include <mye/core/ScriptModule.h>
#include <mye/core/String.h>

#include "BehaviourScript.h"
#include "ProcedureScript.h"
#include "ScriptResourceLoader.h"
#include "Utils.h"

#include <lua.hpp>

#include <lua++11/lua++11.h>

//#ifdef MYE_DEBUG
//#define MYE_LUA_SCRIPT_EXTENSION ".lua"
//#else
//#define MYE_LUA_SCRIPT_EXTENSION ".luac"
//#endif

#define MYE_LUA_SCRIPT_EXTENSION ".lua"


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

			void Init(mye::core::GameObjectHandle hObj);
			void Finalize(mye::core::GameObjectHandle hObj);

			void Update(mye::core::GameObjectsModule::Iterator it);

			lua_State * GetLuaState(void);

			const mye::core::String & GetScriptDirectory(void) const;
			mye::core::String GetLastError(void) const;

			BehaviourScriptPointer      LoadBehaviour(const mye::core::String &name);
			ProcedureScriptPointer      LoadProcedure(const mye::core::String &name);
			ScriptResourceLoaderPointer LoadScriptResourceLoader(const mye::core::String &name);

			bool RunFile(const mye::core::String &file);
			bool RunString(const mye::core::String &code);

		protected:

			mye::core::Script * CreateImpl(const mye::core::String &name,
				mye::core::ManualResourceLoader *manual,
				const mye::core::Parameters &params);

			void FreeImpl(mye::core::Resource* resource);

		private:

			void OpenAllLibraries(void);

			luapp11::State    m_state;
			
			mye::core::String m_lastError;
			mye::core::String m_scriptDirectory;

		};
		

	}

}