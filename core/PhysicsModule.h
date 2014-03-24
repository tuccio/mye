#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Module.h"
#include "Time.h"

namespace mye
{

	namespace core
	{

		class PhysicsModule :
			Module
		{

		public:

			PhysicsModule(void);
			~PhysicsModule(void);

			bool Init(void);
			void ShutDown(void);

			void Update(FloatSeconds dt);

			void AddRigidBody(btRigidBody *rigidbody);
			void RemoveRigidBody(btRigidBody *rigidbody);

			mye::math::Vector3 GetGravity(void) const;
			void SetGravity(const mye::math::Vector3 &g);

		private:

			btDefaultCollisionConfiguration        *m_config;
			btCollisionDispatcher                  *m_dispatcher;
			btBroadphaseInterface                  *m_cache;
			btSequentialImpulseConstraintSolver    *m_solver;
			btDiscreteDynamicsWorld                *m_world;
			//btAlignedObjectArray<btCollisionShape*> m_shapes;

		};

	}

}
