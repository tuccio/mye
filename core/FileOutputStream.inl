namespace mye
{

	namespace core
	{

		bool FileOutputStream::IsOpen(void) const
		{
			return m_file != nullptr;
		}

		void FileOutputStream::Close(void)
		{
			fclose(m_file);
			m_file = nullptr;
		}

		FileOutputStream& FileOutputStream::operator<< (const String &s)
		{
			fwrite(s.CString(), sizeof(char), s.Length(), m_file);
			return *this;
		}

		FileOutputStream& FileOutputStream::operator<< (int x)
		{
			return *this << ToString(x);
		}

		FileOutputStream& FileOutputStream::operator<< (unsigned int x)
		{
			return *this << ToString(x);
		}

		FileOutputStream& FileOutputStream::operator<< (float x)
		{
			return *this << ToString(x);
		}

		FileOutputStream& FileOutputStream::operator<< (double x)
		{
			return *this << ToString(x);
		}

		FileOutputStream::operator bool () const
		{
			return m_file != nullptr;
		}

	}

}