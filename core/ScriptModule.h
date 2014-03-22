#pragma once

#include "Module.h"
#include "Time.h"

namespace mye
{

	namespace core
	{

		class ScriptModule :
			public Module
		{

		public:

			virtual void Preupdate(FloatSeconds dt) = 0;

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
