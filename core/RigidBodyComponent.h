#pragma once

#include "BulletCollisionShape.h"
#include "Component.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <mye/core/AlignedAllocator.h>
#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

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

		__MYE_CLASS_ALIGNED_16(RigidBodyComponent)
			: public Component
		{

		public:

			__MYE_DECLARE_ALIGNED_16

				//void* operator new (size_t, void *p) { return p; }

			RigidBodyComponent(void);

			RigidBodyComponent(
				BulletCollisionShapePointer shape,
				mye::math::Real mass);

			RigidBodyComponent(
				BulletCollisionShapePointer shape,
				mye::math::Real mass,
				const mye::math::Vector3 &position,
				const mye::math::Quaternion &orientation);

			//RigidBodyComponent(const RigidBodyComponent &rb);

			~RigidBodyComponent(void);

			mye::math::Vector3 GetVelocity(void) const;
			void SetVelocity(const mye::math::Vector3 &v);

			mye::math::Vector3 GetPosition(void) const;
			void SetPosition(const mye::math::Vector3 &v);

			mye::math::Quaternion GetOrientation(void) const;
			void SetOrientation(const mye::math::Quaternion &v);

			mye::math::Real GetMass(void) const;
			void SetMass(const mye::math::Real &mass);

			BulletCollisionShapePointer GetCollisionShape(void) const;
			void SetCollisionShape(BulletCollisionShapePointer shape);

			mye::math::Matrix4 GetWorldMatrix(void) const;

			RigidBodyComponent* Clone(void) const;

			void OnAttach(GameObject *go);
			void OnDetach(void);

			btRigidBody* GetRigidBody(void);

		private:

			friend class MotionState;
			
			BulletCollisionShapePointer m_shape;

			MotionState                 m_motionState;
			btRigidBody                 m_rigidbody;

			btTransform                 m_initialTransform;

			/*mye::math::Vector3          m_initialPosition;
			mye::math::Quaternion       m_initialOrientation;*/

		};

	}
}
