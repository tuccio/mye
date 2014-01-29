namespace mye
{

	namespace core
	{

		template <typename T>
		VariableComponent<T>::VariableComponent(const std::string &name) :
			Component(ComponentTypes::COMPONENT_VARIABLE,
			name)
		{
		}

		template <typename T>
		VariableComponent<T>::~VariableComponent(void)
		{
		}

		template <typename T>
		VariableComponent<T>* VariableComponent<T>::Clone(void) const
		{
			return new VariableComponent<T>(*this);
		}

		template <typename T>
		const T& VariableComponent<T>::Get(void) const
		{
			return m_variable;
		}

		template <typename T>
		void VariableComponent<T>::Set(const T& v)
		{
			m_variable = v;
		}

		template <typename T>
		std::type_index VariableComponent<T>::GetVariableType(void) const
		{
			return typeid(T);
		}

	}

}

