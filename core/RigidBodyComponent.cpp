#include "RigidBodyComponent.h"

#include "Game.h"

using namespace mye::core;
using namespace mye::math;

RigidBodyComponent::RigidBodyComponent(BulletCollisionShapePointer shape,
									   mye::math::Real mass,
									   const Vector3 &position,
									   const Quaternion &orientation) :
	Component(ComponentTypes::RIGIDBODY, "rigidbody"),
	m_motionState(nullptr),
	m_rigidbody(nullptr),
	m_shape(shape)
{
// 	m_motionState = new btDefaultMotionState(
// 		btTransform(
// 			btQuaternion(orientation.x(), orientation.y(), orientation.z()),
// 			btVector3(position.x(), position.y(), position.z())
// 		)
// 	);


	m_shape->Load();

	m_motionState = new btDefaultMotionState(
		btTransform(
			btQuaternion(orientation.x(), orientation.y(), orientation.z(), orientation.w()),
			btVector3(position.x(), position.y(), position.z())
		)
	);

	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, m_motionState, m_shape->GetShape());

// 	btVector3    btPosition(position.x(), position.y(), position.z());
// 	btQuaternion btRotation(orientation.x(), orientation.y(), orientation.z(), orientation.w());

	//constructionInfo.m_startWorldTransform = btTransform(btRotation, btPosition);

	m_rigidbody = new btRigidBody(constructionInfo);
	
	Game::GetSingleton().GetPhysicsModule()->AddRigidBody(m_rigidbody);

}

RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent &rb) :
	Component(rb),
	m_motionState(new btDefaultMotionState(*(rb.m_motionState))),
	m_shape(rb.m_shape)
{

// 	btRigidBody::btRigidBodyConstructionInfo constructionInfo(
// 		btScalar(1) / rb.m_rigidbody->getInvMass(),
// 		m_motionState,
// 		m_shape->GetShape());
// 
// 	m_rigidbody = new btRigidBody(constructionInfo);

	m_rigidbody = new btRigidBody(*rb.m_rigidbody);
	m_rigidbody->setMotionState(m_motionState);

	Game::GetSingleton().GetPhysicsModule()->AddRigidBody(m_rigidbody);

}


RigidBodyComponent::~RigidBodyComponent(void)
{
	
	if (m_rigidbody)
	{
		Game::GetSingleton().GetPhysicsModule()->RemoveRigidBody(m_rigidbody);
		delete m_motionState;
		delete m_rigidbody;
	}

}

RigidBodyComponent* RigidBodyComponent::Clone(void) const
{
	return new RigidBodyComponent(*this);
}

mye::math::Vector3 RigidBodyComponent::GetVelocity(void) const
{
	const btVector3 v = m_rigidbody->getLinearVelocity();
	return mye::math::Vector3(v.x(), v.y(), v.z());
}

void RigidBodyComponent::SetVelocity(const mye::math::Vector3 &v)
{
	m_rigidbody->setLinearVelocity(btVector3(v.x(), v.y(), v.z()));
}

mye::math::Vector3 RigidBodyComponent::GetPosition(void) const
{
	btTransform t;
	btVector3 &x = m_motionState->m_graphicsWorldTrans.getOrigin();
	return mye::math::Vector3(x.x(), x.y(), x.z());
}

void RigidBodyComponent::SetPosition(const mye::math::Vector3 &x)
{
	mye::math::Vector3 translation = x - GetPosition();
	m_rigidbody->translate(btVector3(translation.x(), translation.y(), translation.z()));
}

mye::math::Matrix4 RigidBodyComponent::GetWorldMatrix(void) const
{

	btTransform  &t = m_rigidbody->getWorldTransform();
	btVector3    &x = t.getOrigin();
	btQuaternion &q = t.getRotation();

	mye::math::Vector3    mt(x.x(), x.y(), x.z());
	mye::math::Quaternion mq(q.w(), q.x(), q.y(), q.z());

	return RotationTranslationMatrix4(mq, mt);

}