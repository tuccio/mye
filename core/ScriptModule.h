#pragma once

#include "Module.h"

namespace mye
{

	namespace core
	{

		class ScriptModule :
			public Module
		{

		};

	}

}

/*

#include <string>

namespace mye
{

	namespace core
	{

		template <typename ConcreteFunctionCaller>
		class Script;

		template <typename ConcreteFunctionCaller>
		class ScriptModule :
			public IScriptModule
		{

		public:

			ScriptModule(void);
			~ScriptModule(void);

			Script<ConcreteFunctionCaller> LoadClass(const String &filename);
			Script<ConcreteFunctionCaller> LoadProcedure(const String &filename);

			bool Run(const String &filename);

		};

	}

}

#include "ScriptModule.inl"
*/
