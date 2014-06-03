#pragma once

#include <btBulletCollisionCommon.h>
#include "Resource.h"
#include "ResourceManager.h"

namespace mye
{

	namespace core
	{

		class BulletCollisionShape :
			public Resource
		{

		public:

			BulletCollisionShape(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual);

			~BulletCollisionShape(void);

			btCollisionShape* GetShape(void);

			void Clear(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);
			size_t CalculateSizeImpl(void);

		private:

			btCollisionShape *m_shape;

		};

		typedef boost::shared_ptr<BulletCollisionShape>  BulletCollisionShapePointer;

		class BulletCollisionShapeManager :
			public ResourceManager
		{

		public:

			BulletCollisionShapeManager(void);

		protected:

			BulletCollisionShape* CreateImpl(const String &name,
				ManualResourceLoader *manual,
				const Parameters &params);

			virtual void FreeImpl(Resource* resource);

		};

	}

}
