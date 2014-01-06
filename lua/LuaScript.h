#pragma once

#include <mye/core/Script.h>
#include <lua.hpp>
#include <memory>

#include "LuaRegistryReference.h"

namespace mye
{
	namespace lua
	{
		struct LuaScriptCaller;
		typedef mye::core::Script<LuaScriptCaller> LuaScript;
	}
}

namespace mye
{

	namespace core
	{

		template <>
		class ScriptModule<mye::lua::LuaScriptCaller>;

		template <>
		class Script<mye::lua::LuaScriptCaller>
		{

		public:

			enum Type
			{
				CLASS,
				PROCEDURE
			};

			Script(mye::core::ScriptModule<mye::lua::LuaScriptCaller> &module, Type type);
			Script(mye::core::ScriptModule<mye::lua::LuaScriptCaller> &module, Type type, int index);

			ScriptModule<mye::lua::LuaScriptCaller> GetModule(void) const;

			Type GetType(void) const;

			~Script(void);

			std::shared_ptr<const mye::lua::LuaRegistryReference> GetReference(void) const;	

			bool Run(void) const;

			template <typename R>
			R Call(const std::string &f) const;

		private:

			ScriptModule<mye::lua::LuaScriptCaller> &_module;
			Type _type;
			std::shared_ptr<mye::lua::LuaRegistryReference> _ref;

		};

	}
}

#include "LuaScript.inl"