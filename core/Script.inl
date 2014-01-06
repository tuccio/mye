namespace mye
{

	namespace core
	{

		template <typename ConcreteFunctionCaller>
		Script<ConcreteFunctionCaller>::Script(ScriptModule<ConcreteFunctionCaller> &module,
			Type type) :
			_module(module),
			_type(type)
		{
		}

		template <typename ConcreteFunctionCaller>
		Script<ConcreteFunctionCaller>::~Script(void)
		{
		}

		template <typename ConcreteFunctionCaller>
		ScriptModule<ConcreteFunctionCaller>& Script<ConcreteFunctionCaller>::GetModule(void) const
		{
			return _module;
		}

		template <typename ConcreteFunctionCaller>
		template <typename R>
		R Script<ConcreteFunctionCaller>::Call(const std::string &f) const
		{
			return ConcreteFunctionCaller::Call<R>(_module, *this, f);
		}

		template <typename ConcreteFunctionCaller>
		bool Script<ConcreteFunctionCaller>::Run(void) const
		{
			return ConcreteFunctionCaller::Run(_module, *this);
		}

	}

}
