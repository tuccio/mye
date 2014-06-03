namespace mye
{

	namespace core
	{


		const char* String::CString(void) const
		{
			return (m_string ? m_string : (const char*) &m_string);
		}

		char& String::operator[] (Position i)
		{
			assert(i < m_size);
			return *(m_string + i);
		}

		const char& String::operator[] (Position i) const
		{
			assert(i < m_size);
			return *(m_string + i);
		}

		String String::operator+ (const String &s) const
		{
			String r(m_length + s.m_length);
			memcpy(r.m_string, m_string, m_length);
			memcpy(r.m_string + m_length, s.m_string, s.m_length);
			r.m_length = m_length + s.m_length;
			return r;
		}

		String String::operator+ (const char* s) const
		{
			size_t slen = strlen(s);
			String r(m_length + slen);
			memcpy(r.m_string, m_string, m_length);
			memcpy(r.m_string + m_length, s, slen);
			r.m_length = m_length + slen;
			return r;
		}

		String String::operator+ (char c) const
		{
			String r(m_length + 1);
			memcpy(r.m_string, m_string, m_length);
			r.m_string[m_length] = c;
			r.m_length = m_length + 1;
			return r;
		}

		void String::UpdateLength(void)
		{
			m_length = strlen(m_string);
		}

		size_t String::Length(void) const
		{
			return m_length;
		}

		String& String::operator= (const String &s)
		{

			if (s.m_length == 0)
			{
				Clear();
			}
			else if (s.m_length + 1 > m_size)
			{

				delete[] m_string;

				m_length = s.m_length;
				m_size = m_length + 1;
				m_string = new char[m_size];

				memcpy(m_string, s.m_string, m_size);

			}
			else
			{
				m_length = s.m_length;
				memcpy(m_string, s.m_string, m_length + 1);
			}

			return *this;
			

		}

		String& String::operator+= (const String &s)
		{

			size_t oldLength = m_length;
			m_length = oldLength + s.m_length;

			if (m_length + 1 > m_size)
			{

				m_size = m_length + 1;

				char *oldString = m_string;

				m_string = new char[m_size];
				memcpy(m_string, oldString, oldLength);

				delete[] oldString;

			}

			memcpy(m_string + oldLength, s.m_string, s.m_length);
			m_string[m_length] = 0;

			return *this;

		}

		String& String::operator+= (char c)
		{

			size_t oldLength = m_length;
			m_length = oldLength + 1;

			if (m_length + 1 > m_size)
			{

				m_size = m_length + 1;

				char *oldString = m_string;

				m_string = new char[m_size];
				memcpy(m_string, oldString, oldLength);

				delete[] oldString;

			}

			m_string[m_length - 1] = c;
			m_string[m_length] = 0;

			return *this;

		}

		bool String::operator== (const String &s) const
		{

			bool equals = false;

			if (s.m_length == m_length)
			{

				equals = true;
				
				for (unsigned int i = 0; i < m_length; i++)
				{
					if (s.m_string[i] != m_string[i])
					{
						equals = false;
						break;
					}
				}

			}

			return equals;

		}

		bool String::operator!= (const String &s) const
		{
			return !(*this == s);
		}

		bool String::operator== (const char *s) const
		{

			const char *t = m_string;

			while (*t && *s)
			{
				if (*(t++) != *(s++))
				{
					return false;
				}
			}

			return true;

		}

		bool String::operator!= (const char *s) const
		{
			return !(*this == s);
		}

		bool String::operator< (const String &s) const
		{

			bool less = false;
			bool equal = true;

			unsigned int m = (s.m_length < m_length ? s.m_length : m_length);

			for (unsigned int i = 0; i < m; i++)
			{

				if (s.m_string[i] < m_string[i])
				{
					less = true;
					equal = false;
					break;
				}
				else if (s.m_string[i] > m_string[i])
				{
					equal = false;
					break;
				}

			}

			return (equal && m_length < s.m_length ? true : less);

		}

		bool String::IsEmpty(void) const
		{
			return m_length == 0;
		}

		String::Position String::FindFirst(const String &s) const
		{

			String::Position p = String::Null;
			int l = m_length - s.m_length;
			
			for (int i = 0; i < l; i++)
			{

				if (!strncmp(m_string + i, s.m_string, s.m_length))
				{
					p = i;
					break;
				}

			}

			return p;

		}

		String::Position String::FindLast(const String &s) const
		{

			String::Position p = String::Null;
			int l = m_length - s.m_length;

			for (int i = l - 1; i >= 0; i--)
			{

				if (!strncmp(m_string + i, s.m_string, s.m_length))
				{
					p = i;
					break;
				}

			}

			return p;

		}

		String operator+ (const char *s1, const String &s2)
		{
			int s1len = strlen(s1);
			String r(s1len + s2.Length());
			memcpy(r.m_string, s1, s1len);
			memcpy(r.m_string + s1len, s2.m_string, s2.m_length);
			r.m_length = s1len + s2.Length();
			return r;
		}

		String ToString(int x)
		{

			/*String s(__MYE_STRING_INTMAXLEN);

			bool negative = false;

			if (x < 0)
			{
				negative = true;
				x = -x;
			}

			while (x)
			{

				char c = '0' + (x % 10);
				x /= 10;

				s += c;

			}

			if (negative)
			{
				s += '-';
			}

			s.FlipInPlace();

			return s;*/

			return String(std::to_string(x).c_str());

		}

		String ToString(unsigned int x)
		{

			/*String s(__MYE_STRING_INTMAXLEN);

			while (x)
			{

				char c = '0' + (x % 10);
				x /= 10;

				s += c;

			}

			s.FlipInPlace();

			return s;*/

			return String(std::to_string(x).c_str());

		}

		String ToString(float x)
		{
			return String(std::to_string(x).c_str());
		}

		String ToString(double x)
		{
			return String(std::to_string(x).c_str());
		}

		String ToString(bool x)
		{
			return (x ? String("true") : String("false"));
		}

		template <typename T>
		String ToString(const mye::math::Matrix<T, 3, 1> &v)
		{

			String r(32);

			r = "[";
			r += ToString(v.x());
			r += ", ";
			r += ToString(v.y());
			r += ", ";
			r += ToString(v.z());
			r += "]";

			return r;

		}

		template <typename T>
		String ToString(const mye::math::QuaternionTempl<T> &q)
		{

			String r(48);

			r = "[";
			r += ToString(q.w());
			r += "; ";
			r += ToString(q.x());
			r += ", ";
			r += ToString(q.y());
			r += ", ";
			r += ToString(q.z());
			r += "]";

			return r;

		}

		template <typename T>
		mye::math::Matrix<T, 2, 1> ParseVector2(const String &s)
		{

			mye::math::Matrix<T, 2, 1> v;
			std::stringstream ss(s.CString());

			ss >> v.x() >> v.y();

			return v;

		}

		template <typename T>
		mye::math::Matrix<T, 3, 1> ParseVector3(const String &s)
		{

			mye::math::Matrix<T, 3, 1> v;
			std::stringstream ss(s.CString());

			ss >> v.x() >> v.y() >> v.z();

			return v;

		}

		template <typename T>
		mye::math::Matrix<T, 4, 1> ParseVector4(const String &s)
		{

			mye::math::Matrix<T, 4, 1> v;
			std::stringstream ss(s.CString());

			ss >> v.x() >> v.y() >> v.z() >> v.w();

			return v;

		}

		template <typename T>
		mye::math::QuaternionTempl<T> ParseQuaternion(const String &s)
		{

			mye::math::QuaternionTempl<T> q;
			std::stringstream ss(s.CString());

			ss >> q.w() >> q.x() >> q.y() >> q.z();

			return q;

		}

		template <typename T>
		T ParseType(const String &s)
		{

			T x;
			std::stringstream ss(s.CString());

			ss >> x;

			return x;

		}

		String::Iterator& String::Iterator::operator++ (void)
		{
			m_it++;
			return *this;
		}

		String::Iterator& String::Iterator::operator-- (void)
		{
			m_it--;
			return *this;
		}

		String::Iterator String::Iterator::operator+ (size_t pos)
		{
			return Iterator(m_it + pos);
		}

		String::Iterator String::Iterator::operator- (size_t pos)
		{
			return Iterator(m_it - pos);
		}

		int String::Iterator::operator- (Iterator it)
		{
			return m_it - it.m_it;
		}

		bool String::Iterator::operator== (const String::Iterator &it) const
		{
			return m_it == it.m_it;
		}

		bool String::Iterator::operator!= (const String::Iterator &it) const
		{
			return m_it != it.m_it;
		}

		char& String::Iterator::operator* (void) const
		{
			return *m_it;
		}

		String::Iterator String::begin(void)
		{
			return m_string;
		}

		String::Iterator String::end(void)
		{
			return m_string + m_length;
		}

	}

}

namespace std
{

	/* Hash for mye::core::String */

	template <>
	struct hash<mye::core::String>
	{

		size_t operator() (const mye::core::String &s) const
		{

			// MurmurHash2, by Austin Appleby

			// TODO: Randomize the seed for every execution

			static unsigned int seed = 0x0;
			unsigned int len = s.m_length;

			// 'm' and 'r' are mixing constants generated offline.
			// They're not really 'magic', they just happen to work well.

			const unsigned int m = 0x5bd1e995;
			const int r = 24;

			// Initialize the hash to a 'random' value

			unsigned int h = seed ^ len;

			// Mix 4 bytes at a time into the hash

			const unsigned char * data = (const unsigned char *) s.m_string;

			while(len >= 4)
			{

				unsigned int k = *(unsigned int *) data;

				k *= m; 
				k ^= k >> r; 
				k *= m; 

				h *= m; 
				h ^= k;

				data += 4;
				len -= 4;

			}

			// Handle the last few bytes of the input arRayt

			switch (len)
			{
			case 3: h ^= data[2] << 16;
			case 2: h ^= data[1] << 8;
			case 1: h ^= data[0];
				h *= m;
			};

			// Do a few final mixes of the hash to ensure the last few
			// bytes are well-incorporated.

			h ^= h >> 13;
			h *= m;
			h ^= h >> 15;

			return h;

		}

		

	};

}