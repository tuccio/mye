#pragma once

#include "Script.h"

namespace mye
{

	namespace lua
	{

		class ProcedureScript :
			public Script
		{

		public:

			ProcedureScript(void);
			ProcedureScript(LuaModule &luaModule,
				const mye::core::String &name);

			~ProcedureScript(void);

			inline void Run(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

		};

		typedef boost::shared_ptr<ProcedureScript> ProcedureScriptPointer;

	}

}

#include "ProcedureScript.inl"
