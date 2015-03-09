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

			Component(ComponentTypes type, const String & name);

			virtual ~Component(void) = 0;

			ComponentTypes GetComponentType(void) const;

			inline GameObject * GetOwner(void)
			{
				return m_owner;
			}

			virtual void OnAttach(GameObject * gameObject);
			virtual void OnDetach(void);

		protected:

			friend class GameObject;

			ComponentTypes   m_type;
			GameObject     * m_owner;

		};
	}
}

