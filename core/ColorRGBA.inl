namespace mye
{

	namespace core
	{

		const float* ColorRGBA::Data(void) const
		{
			return m_data;
		}

		float* ColorRGBA::Data(void)
		{
			return m_data;
		}

		float& ColorRGBA::r(void)
		{
			return m_data[0];
		}

		const float& ColorRGBA::r(void) const
		{
			return m_data[0];
		}

		float& ColorRGBA::g(void)
		{
			return m_data[1];
		}

		const float& ColorRGBA::g(void) const
		{
			return m_data[1];
		}

		float& ColorRGBA::b(void)
		{
			return m_data[2];
		}

		const float& ColorRGBA::b(void) const
		{
			return m_data[2];
		}

		float& ColorRGBA::a(void)
		{
			return m_data[3];
		}

		const float& ColorRGBA::a(void) const
		{
			return m_data[3];
		}

	}

}