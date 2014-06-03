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

		class RigidBodyComponent;

		class MotionState :
			public btMotionState
		{

		public:

			MotionState(RigidBodyComponent *rb);

			void getWorldTransform(btTransform& worldTrans) const;
			void setWorldTransform(const btTransform& worldTrans);

		private:

			RigidBodyComponent *m_rigidbody;

		};

		class RigidBodyComponent
			: public Component
		{

		public:

			RigidBodyComponent(BulletCollisionShapePointer shape,
				mye::math::Real mass);

			RigidBodyComponent(const RigidBodyComponent &rb);

			~RigidBodyComponent(void);

			mye::math::Vector3 GetVelocity(void) const;
			void SetVelocity(const mye::math::Vector3 &v);

			mye::math::Vector3 GetPosition(void) const;
			void SetPosition(const mye::math::Vector3 &v);

			mye::math::Quaternion GetOrientation(void) const;
			void SetOrientation(const mye::math::Quaternion &v);

			mye::math::Real GetMass(void) const;
			void SetMass(const mye::math::Real &mass);

			mye::math::Matrix4 GetWorldMatrix(void) const;

			RigidBodyComponent* Clone(void) const;

		private:

			friend class MotionState;

			btRigidBody                   *m_rigidbody;
			MotionState                   *m_motionState;
			BulletCollisionShapePointer    m_shape;

		};

	}
}
