#pragma once

#include <mye/core/Script.h>
#include <lua++11/lua++11.h>

namespace mye
{

	namespace lua
	{

		class ProcedureScript :
			public mye::core::Script
		{

		public:

			ProcedureScript(void);
			ProcedureScript(const mye::core::String &name);

			~ProcedureScript(void);

			inline void Run(void)
			{
				m_script.Call<void>();
			}

			inline const mye::core::String& ProcedureScript::GetLastError(void) const
			{
				return m_error;
			}

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

		private:

			mye::core::String m_error;
			luapp11::Object   m_script;


		};

		typedef std::shared_ptr<ProcedureScript> ProcedureScriptPointer;

	}

}
