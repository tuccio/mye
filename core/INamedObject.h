#pragma once

#include <string>

namespace mye
{

	namespace core
	{

		class INamedObject
		{

		public:

			INamedObject(void);
			INamedObject(const std::string &name);

			~INamedObject(void);

			const std::string& GetName(void) const;

		protected:

			void SetName(const std::string &name);

		private:

			std::string m_name;

		};

	}

}
