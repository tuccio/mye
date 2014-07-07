#pragma once

#include "Component.h"
#include <mye/math/Geometry.h>

#include <typeindex>

namespace mye
{

	namespace core
	{

		template <typename T>
		class VariableConstraint;

		template <typename T>
		class VariableComponent :
			public Component
		{

		public:

			VariableComponent(const String &name);
/*
			VariableComponent(const String &name,
				const std::vector<VariableConstraint<T>*> &constraints)*/
			~VariableComponent(void);

			VariableComponent* Clone(void) const;

			virtual std::type_index GetVariableType(void) const;

			T* GetPointer(void);

			const T& Get(void) const;
			void Set(const T &v);

/*
			void AddConstraint(const VariableConstraint<T>& constraint);
			bool FitsConstraints(const T &value) const;
			void ClearConstraints(void);*/

		private:

			T m_variable;
			//std::vector<VariableConstraint<T>*> m_constraints;

		};

		/*template <>
		class __declspec(align(16)) VariableComponent<Transform> :
			public Component
		{

		public:

			VariableComponent(const String &name);
			~VariableComponent(void);

			VariableComponent<Transform>* Clone(void) const;

			virtual std::type_index GetVariableType(void) const;

			const Transform& Get(void) const;
			void Set(const Transform &t);

		private:

			Transform m_transform;

			template <typename T>
			class VariableConstraint
			{

			public:

				virtual bool fits(const T& value) const;

			};

			template <typename T>
			class InclusiveMaxConstraint
			{

			public:

				InclusiveMaxConstraint(const T &max);

				bool fits(const T &value) const
				{
					return value <= m_max;
				}

			private:

				T m_max;

			};

			template <typename T>
			class InclusiveMinConstraint
			{

			public:

				InclusiveMinConstraint(const T &min);

				bool fits(const T &value) const
				{
					return value >= m_min;
				}

			private:

				T m_min;

			};

		};*/


	}

}

#include "VariableComponent.inl"
