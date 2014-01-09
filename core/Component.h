#pragma once

#include "Cloneable.h"
#include "ComponentTypes.h"

#include <string>

namespace mye
{

	namespace core
	{

		class Component : public Cloneable
		{

		public:

			Component(ComponentTypes type);
			virtual ~Component(void) = 0;

			ComponentTypes GetComponentType(void);

		protected:

			ComponentTypes m_type;

		};
	}
}

