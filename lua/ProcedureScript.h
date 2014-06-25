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

			inline const mye::core::String& GetLastError(void) const;

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

		private:

			mye::core::String m_error;

		};

		typedef boost::shared_ptr<ProcedureScript> ProcedureScriptPointer;

	}

}

#include "ProcedureScript.inl"
