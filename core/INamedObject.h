#pragma once

#include "String.h"

namespace mye
{

	namespace core
	{

		class INamedObject
		{

		public:

			INamedObject(void);
			INamedObject(const String & name);

			~INamedObject(void);

			const String & GetName(void) const;

		protected:

			void SetName(const String & name);
			String m_name;			

		};

	}

}
