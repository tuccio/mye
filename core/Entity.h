#pragma once

#include "String.h"
#include "GameObject.h"
#include "Resource.h"

#include "EntityInstantiation.h"

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
				const String & name,
				ManualResourceLoader *manual);

			bool Instantiate(GameObject * object);

			void Clear(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			entity::EntityDefinition m_entity;

		};

		typedef std::shared_ptr<Entity> EntityPointer;

	}

}