#pragma once

#include "Component.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <mye/math/Math.h>

namespace mye
{

	namespace core
	{

		class RigidBodyComponent
			: public Component
		{

		public:

			RigidBodyComponent(btCollisionShape *shape,
				mye::math::Real mass,
				const mye::math::Vector3 &position = mye::math::Vector3(0, 0, 0),
				const mye::math::Quaternion &orientation = mye::math::Quaternion(1, 0, 0, 0));

			~RigidBodyComponent(void);

			RigidBodyComponent* Clone(void) const;

		private:

			btRigidBody          m_rigidbody;
			btDefaultMotionState m_motionState;
			btCollisionShape    *m_shape;

		};

	}
}
