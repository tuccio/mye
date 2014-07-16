#pragma once

#include <string>

namespace mye
{

	namespace luab
	{

		template <typename T>
		class Class
		{

		public:

			Class(const std::string &name);
			~Class(void);

			const std::string& GetName(void);

		private:

			const std::string m_name;

		};

	}

}

#include "Class.inl"