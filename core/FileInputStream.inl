namespace mye
{

	namespace core
	{

		bool FileInputStream::IsOpen(void) const
		{
			return m_file != nullptr;
		}

		bool FileInputStream::IsOver(void) const
		{
			return m_eof && m_begin == m_end;
		}

	}

}