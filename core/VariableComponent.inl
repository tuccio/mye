namespace mye
{

	namespace core
	{

		template <typename T>
		VariableComponent<T>::VariableComponent(void) :
			Component(ComponentTypes::VARIABLE_COMPONENT)
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
		T VariableComponent<T>::Get(void) const
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
