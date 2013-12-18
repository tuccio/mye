#pragma once

#include "Cloneable.h"

#include <string>

namespace mye
{

	namespace core
	{

		class Component : public Cloneable
		{

		public:

			static const char* Name(void);

			Component(const std::string &name);
			virtual ~Component(void) = 0;

			virtual bool IsNull(void) const;

			const std::string& GetName(void) const;

			virtual Component& operator= (const Component& component) = 0;

		private:

			const std::string _name;

		};
	}
}

