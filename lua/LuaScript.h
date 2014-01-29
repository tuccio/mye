#pragma once

#include <memory>
#include <string>

#include "LuaRegistryReference.h"
#include "LuaModule.h"

namespace mye
{
	namespace lua
	{

		/*struct LuaScriptCaller;
		typedef mye::core::Script<LuaScriptCaller> LuaScript;*/

		class LuaScript
		{

		public:

			typedef std::shared_ptr<mye::lua::LuaRegistryReference> Reference;

			enum Type
			{
				BEHAVIOUR,
				PROCEDURE
			};

			LuaScript(LuaModule &module, Type type);
			LuaScript(LuaModule &module, Type type, int index);

			~LuaScript(void);

			Type GetType(void) const;

			Reference GetReference(void) const;

			bool Run(void) const;

			template <typename R>
			R Call(const std::string &f) const;

			template <typename R, typename T1>
			R Call(const std::string &f, T1 arg1) const;

		private:

			LuaModule &m_module;
			Type m_type;
			Reference m_ref;

		};


	}
}

#include "LuaScript.inl"