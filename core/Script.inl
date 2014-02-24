/*
namespace mye
{

	namespace core
	{

		template <typename ConcreteFunctionCaller>
		Script<ConcreteFunctionCaller>::Script(ScriptModule<ConcreteFunctionCaller> &module,
			Type type) :
			m_module(module),
			m_type(type)
		{
		}

		template <typename ConcreteFunctionCaller>
		Script<ConcreteFunctionCaller>::~Script(void)
		{
		}

		template <typename ConcreteFunctionCaller>
		ScriptModule<ConcreteFunctionCaller>& Script<ConcreteFunctionCaller>::GetModule(void) const
		{
			return m_module;
		}

		template <typename ConcreteFunctionCaller>
		template <typename R>
		R Script<ConcreteFunctionCaller>::Call(const String &f) const
		{
			return ConcreteFunctionCaller::Call<R>(m_module, *this, f);
		}

		template <typename ConcreteFunctionCaller>
		bool Script<ConcreteFunctionCaller>::Run(void) const
		{
			return ConcreteFunctionCaller::Run(m_module, *this);
		}

	}

}
*/
