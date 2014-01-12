#pragma once

#include "Component.h"
#include "Transform.h"

#include <typeindex>

namespace mye
{

	namespace core
	{

		template <typename T>
		class VariableComponent :
			public Component
		{

		public:

			VariableComponent(const std::string &name);
			~VariableComponent(void);

			VariableComponent* Clone(void) const;

			virtual std::type_index GetVariableType(void) const;

			T Get(void) const;
			void Set(const T &v);

		private:

			T m_variable;

		};

		template <>
		class __declspec(align(16)) VariableComponent<Transform> :
			public Component
		{

		public:

			VariableComponent(const std::string &name);
			~VariableComponent(void);

			VariableComponent<Transform>* Clone(void) const;

			virtual std::type_index GetVariableType(void) const;

			const Transform& Get(void) const;
			void Set(const Transform &t);

			EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		private:

			Transform m_transform;

		};

	}

}

#include "VariableComponent.inl"
