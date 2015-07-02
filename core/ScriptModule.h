#pragma once

#include "EventManager.h"
#include "Module.h"
#include "Time.h"
#include "String.h"

#include "GameObjectsModule.h"

#include <vector>

namespace mye
{

	namespace core
	{

		class ScriptModule :
			public Module
		{

		public:

			virtual bool Init(void) { return false; }
			virtual void Shutdown(void) { }

			virtual void Init(GameObject * object) { }
			virtual void Finalize(GameObject * object) { }

			virtual void Preupdate(FloatSeconds dt) { }
			virtual void Update(void) { }

			virtual void OnEvent(GameObject * object, const IEvent * e) { }

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
