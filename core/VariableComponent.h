#pragma once

#include "Component.h"

#include <map>
#include <string>

#include <memory>

namespace mye
{

	namespace core
	{
		class VariableComponent :
			public Component
		{

		public:

			static const char* Name(void);

			VariableComponent(void);
			~VariableComponent(void);

			void Register(const std::string &name, double initValue = 0);

			double Get(const std::string &name);
			void Set(const std::string &name, double value);

			void Remove(const std::string &name);

			void Clear(void);

			Cloneable* Clone(void) const;

			Component& operator= (const Component& component);

		private:

			std::map<std::string, double> _info;

		};

	}
}
