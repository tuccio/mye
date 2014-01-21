namespace mye
{

	namespace core
	{

		const float* ColorRGB::Data(void) const
		{
			return m_data;
		}

		float* ColorRGB::Data(void)
		{
			return m_data;
		}

		float& ColorRGB::r(void)
		{
			return m_data[0];
		}

		const float& ColorRGB::r(void) const
		{
			return m_data[0];
		}

		float& ColorRGB::g(void)
		{
			return m_data[1];
		}

		const float& ColorRGB::g(void) const
		{
			return m_data[1];
		}

		float& ColorRGB::b(void)
		{
			return m_data[2];
		}

		const float& ColorRGB::b(void) const
		{
			return m_data[2];
		}

	}

}