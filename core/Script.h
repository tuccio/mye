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
			virtual ~Script(void);

			Type GetType(void) const;

			ScriptModule<ConcreteFunctionCaller>& GetModule(void) const;

			template <typename R>
			R Call(const std::string &f) const;

			bool Run(void) const;

		private:

			ScriptModule<ConcreteFunctionCaller> &_module;
			Type _type;

		};

	}

}

#include "Script.inl"
