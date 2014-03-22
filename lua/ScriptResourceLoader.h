#pragma once

#include "BehaviourScript.h"

#include <mye/core/ManualResourceLoader.h>

namespace mye
{

	namespace lua
	{

		class ScriptResourceLoader :
			public mye::core::ManualResourceLoader,
			public BehaviourScript
		{

		public:

			ScriptResourceLoader(LuaModule &luaModule,
				const mye::core::String &name);

			~ScriptResourceLoader(void);

			bool Load(bool background = false);
			void Unload(bool background = false);

			bool Load(Resource *resource);
			void Unload(Resource *resource);

		};

		typedef boost::shared_ptr<ScriptResourceLoader> ScriptResourceLoaderPointer;

	}

}
