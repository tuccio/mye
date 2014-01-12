#pragma once

#include "Cloneable.h"
#include "ComponentTypes.h"
#include "INamedObject.h"

#include <string>

namespace mye
{

	namespace core
	{

		class Component :
			public Cloneable,
			public INamedObject
		{

		public:

			Component(ComponentTypes type,
				const std::string &name);
			virtual ~Component(void) = 0;

			ComponentTypes GetComponentType(void);

		protected:

			ComponentTypes m_type;

		};
	}
}

