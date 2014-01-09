#pragma once

#include "Component.h"
#include <typeindex>

namespace mye
{

	namespace core
	{

		template <typename T>
		class VariableComponent : public Component
		{

		public:

			VariableComponent(void);
			~VariableComponent(void);

			VariableComponent* Clone(void) const;

			virtual std::type_index GetVariableType(void) const;

			T Get(void) const;
			void Set(const T& v);

		private:

			T m_variable;

		};

	}

}

#include "VariableComponent.inl"
