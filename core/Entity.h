#pragma once

#include "String.h"
#include "Components.h"
#include "Resource.h"

namespace mye
{

	namespace core
	{

		class Entity :
			public Resource
		{

		public:

			typedef std::vector<Component*>::iterator Iterator;

			Entity(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual);

			void Insert(Component *c);
			void Clear(void);

			inline Iterator begin(void)
			{
				return m_components.begin();
			}

			inline Iterator end(void)
			{
				return m_components.end();
			}

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			std::vector<Component*> m_components;

		};

		typedef boost::shared_ptr<Entity> EntityPointer;

	}

}