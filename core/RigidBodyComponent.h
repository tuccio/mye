#pragma once

#include "BulletCollisionShape.h"
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

			RigidBodyComponent(BulletCollisionShapePointer shape,
				mye::math::Real mass,
				const mye::math::Vector3 &position = mye::math::Vector3(0, 0, 0),
				const mye::math::Quaternion &orientation = mye::math::Quaternion(1, 0, 0, 0));

			RigidBodyComponent(const RigidBodyComponent &rb);

			~RigidBodyComponent(void);

			mye::math::Vector3 GetVelocity(void) const;
			void SetVelocity(const mye::math::Vector3 &v);

			mye::math::Vector3 GetPosition(void) const;
			void SetPosition(const mye::math::Vector3 &v);

			mye::math::Matrix4 GetWorldMatrix(void) const;

			RigidBodyComponent* Clone(void) const;

		private:

			btRigidBody                   *m_rigidbody;
			btDefaultMotionState          *m_motionState;
			BulletCollisionShapePointer    m_shape;

		};

	}
}
