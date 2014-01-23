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

			Script<ConcreteFunctionCaller> LoadClass(const std::string &filename);
			Script<ConcreteFunctionCaller> LoadProcedure(const std::string &filename);

			bool Run(const std::string &filename);

		};

	}

}

#include "ScriptModule.inl"
*/
