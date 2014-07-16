namespace mye
{

	namespace luab
	{

		template <typename T>
		Class<T>::Class(const std::string &name) :
			m_name(name)
		{

		}

		template<typename T>
		Class<T>::~Class(void)
		{

		}

		template <typename t>
		const std::string& Class<T>::GetName(void)
		{
			return m_name;
		}

	}

}