#pragma once

#include <mye/core/ResourceManager.h>
#include <mye/core/ScriptModule.h>
#include <mye/core/String.h>

#include "BehaviourScript.h"
#include "ProcedureScript.h"
#include "ScriptResourceLoader.h"
#include "Script.h"
#include "Utils.h"

#include <lua.hpp>
#include <luabind/luabind.hpp>

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

			template <typename T>
			T Create(const mye::core::String &name, const std::vector<std::string> &initializer);

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

namespace mye
{

	namespace core
	{

		template <typename T>
		struct ScriptObjectCreator
		{

			static boost::optional<T> Create(const mye::core::String &name, const mye::core::String &initializer)
			{

				// TODO: Sandbox

				lua_State *lua = LPLUASTATE;
				mye::lua::LuaStackCleaner stackCleaner(lua);

				mye::core::String code(511);

				code = "return ";
				code += name;
				code += "(";
				code += initializer;

				/*bool first = true;

				for (auto &s : initializer)
				{

					if (!first)
					{
						code += ",";
					}
					else
					{
						first = false;
					}
					
					code += s.c_str();

				}*/

				code += ")";

				if (luaL_dostring(lua, code.CString()))
				{
					return boost::optional<T>();
				}

				return luabind::object_cast_nothrow<T>(luabind::object(luabind::from_stack(lua, -1)));

			}

		};

	}

}