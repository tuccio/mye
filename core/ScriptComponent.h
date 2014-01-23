#pragma once

#include "Component.h"

#include <mye/lua/LuaScript.h>

namespace mye
{

	namespace core
	{

		class ScriptComponent :
			public Component
		{

		public:

			ScriptComponent(mye::lua::LuaScript script);
			~ScriptComponent(void);

			Cloneable* Clone(void) const;

			inline mye::lua::LuaScript& Script(void);
			inline const mye::lua::LuaScript& Script(void) const;

		private:

			mye::lua::LuaScript m_script;

/*			const Script& Get(void);*/

		};

	}

}

#include "ScriptComponent.inl"