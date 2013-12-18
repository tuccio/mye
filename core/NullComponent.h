#pragma once

#include "Component.h"

namespace mye
{

	namespace core
	{

		class NullComponent : public Component
		{

		public:

			static NullComponent dummy;
			
			static const char* Name(void);

			NullComponent(void);
			~NullComponent(void);

			bool IsNull(void) const;

			Cloneable* Clone(void) const;

			Component& operator= (const Component& component);

		};

	}

}
