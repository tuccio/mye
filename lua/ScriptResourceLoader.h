#pragma once

#include <mye/core/Script.h>
#include <mye/core/ManualResourceLoader.h>

#include <lua++11/lua++11.h>

namespace mye
{

	namespace lua
	{

		class ScriptResourceLoader :
			public mye::core::ManualResourceLoader,
			public mye::core::Script
		{

		public:

			ScriptResourceLoader(const mye::core::String &name);
			~ScriptResourceLoader(void);

			bool Load(mye::core::Resource * resource);
			void Unload(mye::core::Resource * resource);

		private:

			luapp11::Object m_loader;

		};

		typedef std::shared_ptr<ScriptResourceLoader> ScriptResourceLoaderPointer;

	}

}
