#pragma once

#include "Cloneable.h"
#include "ComponentTypes.h"
#include "INamedObject.h"

#include <string>

namespace mye
{

	namespace core
	{

		class GameObject;

		class Component :
			public Cloneable,
			public INamedObject
		{

		public:

			Component(ComponentTypes type,
				const std::string &name);

			virtual ~Component(void) = 0;

			ComponentTypes GetComponentType(void) const;

			inline GameObject* GetOwner(void)
			{
				return m_owner;
			}

		protected:

			friend class GameObject;

			ComponentTypes m_type;
			GameObject *m_owner;

		};
	}
}

