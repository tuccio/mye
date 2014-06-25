#pragma once

#include "Module.h"
#include "Time.h"
#include "String.h"

#include <vector>

#include <boost/optional.hpp>

namespace mye
{

	namespace core
	{

		template <typename T>
		struct ScriptObjectCreator;

		class ScriptModule :
			public Module
		{

		public:

			virtual void Preupdate(FloatSeconds dt) = 0;

			template <typename T>
			boost::optional<T> Create(const String &name, const String &initializer)
			{
				return ScriptObjectCreator<T>::Create(name, initializer);
			}

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
