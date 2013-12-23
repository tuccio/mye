#pragma once

#include "Component.h"
#include "Variable.h"

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

			Variable& Add(const std::string &name);
			bool Contains(const std::string &name) const;

			Variable& operator[] (const std::string &name);
			const Variable& operator[] (const std::string &name) const;

			void Clear(void);

			Cloneable* Clone(void) const;

			Component& operator= (const Component& component);

		private:

			std::map<std::string, Variable> _info;

		};

	}
}
