#include "RigidBodyComponent.h"

#include "Game.h"

using namespace mye::core;
using namespace mye::math;

RigidBodyComponent::RigidBodyComponent(BulletCollisionShapePointer shape,
				   mye::math::Real mass) :
	Component(ComponentTypes::RIGIDBODY, "rigidbody"),
	m_motionState(nullptr),
	m_rigidbody(nullptr),
	m_shape(shape)
{

	m_shape->Load();

	m_motionState = new MotionState(this);

	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, m_motionState, m_shape->GetShape());
	m_rigidbody = new btRigidBody(constructionInfo);
	m_rigidbody->setDeactivationTime(DISABLE_DEACTIVATION);

	Game::GetSingleton().GetPhysicsModule()->AddRigidBody(m_rigidbody);

}

RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent &rb) :
	Component(rb),
	m_shape(rb.m_shape)
{


	m_motionState = new MotionState(this);

	btTransform t;
	rb.m_motionState->getWorldTransform(t);
	m_motionState->setWorldTransform(t);

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
	m_motionState->getWorldTransform(t);
	btVector3 &x = t.getOrigin();
	return mye::math::Vector3(x.x(), x.y(), x.z());
}

void RigidBodyComponent::SetPosition(const mye::math::Vector3 &x)
{
	
	btTransform t;
	m_motionState->getWorldTransform(t);
	t.setOrigin(btVector3(x.x(), x.y(), x.z()));
	//m_rigidbody->translate(btVector3(x.x(), x.y(), x.z()) - t.getOrigin());

	m_rigidbody->setCenterOfMassTransform(t);

}

mye::math::Quaternion RigidBodyComponent::GetOrientation(void) const
{
	btTransform t;
	m_motionState->getWorldTransform(t);
	btQuaternion &x = t.getRotation();
	return mye::math::Quaternion(x.x(), x.y(), x.z(), x.w());
}

void RigidBodyComponent::SetOrientation(const mye::math::Quaternion &x)
{

	btTransform t;
	m_motionState->getWorldTransform(t);
	
	t.setRotation(btQuaternion(x.x(), x.y(), x.z(), x.w()));
	m_rigidbody->setCenterOfMassTransform(t);

}

mye::math::Real RigidBodyComponent::GetMass(void) const
{
	return mye::math::Real(1) / m_rigidbody->getInvMass();
}

void RigidBodyComponent::SetMass(const mye::math::Real &mass)
{
	m_rigidbody->setMassProps(mass, btVector3(0, 0, 0));
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

/* MotionState updates the Transform Component (used for culling and rendering */

MotionState::MotionState(RigidBodyComponent *rb) :
	m_rigidbody(rb)
{

}

// MotionState::MotionState(RigidBodyComponent *rb,
// 						 const mye::math::Vector3 &position,
// 						 const mye::math::Quaternion &orientation) :
// m_rigidbody(rb)
// {
// 
// 	if (m_rigidbody->m_owner)
// 	{
// 		auto *tc = m_rigidbody->m_owner->GetTransformComponent();
// 		tc->SetPosition(position);
// 		tc->SetOrientation(orientation);
// 	}
// 
// }

void MotionState::getWorldTransform(btTransform& worldTrans) const
{

	if (m_rigidbody->m_owner)
	{

		auto *tc = m_rigidbody->m_owner->GetTransformComponent();

		const mye::math::Vector3    &position    = tc->GetPosition();
		const mye::math::Quaternion &orientation = tc->GetOrientation();

		worldTrans.setOrigin(btVector3(position.x(), position.y(), position.z()));
		worldTrans.setRotation(btQuaternion(orientation.x(), orientation.y(), orientation.z(), orientation.w()));

	}

}

void MotionState::setWorldTransform(const btTransform& worldTrans)
{

	if (m_rigidbody->m_owner)
	{
		auto *tc = m_rigidbody->m_owner->GetTransformComponent();

		const btVector3    &btPosition    = worldTrans.getOrigin();
		const btQuaternion &btOrientation = worldTrans.getRotation();

		mye::math::Vector3    position(btPosition.x(), btPosition.y(), btPosition.z());
		mye::math::Quaternion orientation(btOrientation.x(), btOrientation.y(), btOrientation.z(), btOrientation.w());

		tc->SetPosition(mye::math::Vector3(btPosition.x(), btPosition.y(), btPosition.z()));
		tc->SetOrientation(mye::math::Quaternion(btOrientation.w(), btOrientation.x(), btOrientation.y(), btOrientation.z()));
	}

}