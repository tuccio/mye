#pragma once

#include <mye/core/Resource.h>

#include <lua.hpp>

namespace mye
{

	namespace lua
	{

		class LuaModule;

		class Script :
			public mye::core::Resource
		{

		public:

			Script(void);
			Script(LuaModule &luaModule,
				const mye::core::String &name);

			~Script(void);

			void Free(void);

			inline int GetRegistryReference(void);

		protected:

			size_t CalculateSizeImpl(void);

			int m_registryReference;
			lua_State *m_lua;

			mye::core::String m_scriptDirectory;

		};

		typedef boost::shared_ptr<Script> ScriptPointer;

	}

}

#include "Script.inl"