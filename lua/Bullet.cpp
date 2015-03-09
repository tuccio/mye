#include "Bullet.h"
#include "Types.h"
#include "Converters.h"

#include <mye/core/BulletCollisionShape.h>
#include <mye/core/PhysicsModule.h>
#include <mye/core/RigidBodyComponent.h>

using namespace mye::core;
using namespace mye::lua;

namespace mye
{

	namespace lua
	{

		void BindBullet(luapp11::State state)
		{

			using namespace luapp11;

			state
			[

				Namespace("Bullet")
				[

					Class<btCollisionShape>(MYE_LUA_BULLET_SHAPE),

					Class<btSphereShape, btCollisionShape>(MYE_LUA_BULLET_SPHERE),

					Class<btBoxShape, btCollisionShape>(MYE_LUA_BULLET_BOX).
						Constructor<const btVector3&>(),

					Class<BulletCollisionShape, Resource>("__BulletCollisionShape")

				]

			];

		}

	}

}