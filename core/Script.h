/*
#pragma once

#include <string>

namespace mye
{

	namespace core
	{

		template <typename ConcreteFunctionCaller>
		class ScriptModule;

		template <typename ConcreteFunctionCaller>
		class Script
		{

		public:

			enum Type
			{
				CLASS,
				PROCEDURE
			};

			Script(ScriptModule<ConcreteFunctionCaller> &module, Type type);
			~Script(void);

			Type GetType(void) const;

			ScriptModule<ConcreteFunctionCaller>& GetModule(void) const;

			template <typename R>
			R Call(const std::string &f) const;

			bool Run(void) const;

		private:

			ScriptModule<ConcreteFunctionCaller> &m_module;
			Type m_type;

		};

		class Script
		{

		public:

		};

	}

}

#include "Script.inl"
*/
