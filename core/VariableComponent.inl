namespace mye
{

	namespace core
	{

		template <typename T, typename Allocator>
		VariableComponent<T, Allocator>::VariableComponent(const String &name) :
			VariableRTTI(name, VariableTraits<T>::type::value)
		{
		}

		template <typename T, typename Allocator>
		VariableComponent<T, Allocator>::~VariableComponent(void)
		{
		}

		template <typename T, typename Allocator>
		VariableComponent<T, Allocator> * VariableComponent<T, Allocator>::Clone(void) const
		{
			return new VariableComponent<T>(*this);
		}

		template <typename T, typename Allocator>
		T * VariableComponent<T, Allocator>::GetPointer(void)
		{
			return &m_variable;
		}

		template <typename T, typename Allocator>
		const T & VariableComponent<T, Allocator>::Get(void) const
		{
			return m_variable;
		}

		template <typename T, typename Allocator>
		void VariableComponent<T, Allocator>::Set(const T& v)
		{
			m_variable = v;
		}

	}

}