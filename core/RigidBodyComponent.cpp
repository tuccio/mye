#include "RigidBodyComponent.h"

#include "Game.h"

using namespace mye::core;
using namespace mye::math;

RigidBodyComponent::RigidBodyComponent(void) :
	Component(ComponentTypes::RIGIDBODY, "rigidbody"),
	m_motionState(this),
	m_rigidbody(btRigidBody::btRigidBodyConstructionInfo(0.0f, &m_motionState, nullptr)),
	m_initialTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))
{

	m_rigidbody.setActivationState(DISABLE_DEACTIVATION);

}

RigidBodyComponent::RigidBodyComponent(BulletCollisionShapePointer shape,
	mye::math::Real mass) :
	Component(ComponentTypes::RIGIDBODY, "rigidbody"),
	m_shape(shape),
	m_motionState(this),
	m_rigidbody(btRigidBody::btRigidBodyConstructionInfo(mass, &m_motionState, (shape && shape->Load() ? shape->GetShape() : nullptr))),
	m_initialTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))
{

	m_rigidbody.setActivationState(DISABLE_DEACTIVATION);

}

RigidBodyComponent::RigidBodyComponent(
	BulletCollisionShapePointer shape,
	mye::math::Real mass,
	const Vector3 &position,
	const Quaternion &orientation) :
	Component(ComponentTypes::RIGIDBODY, "rigidbody"),
	m_shape(shape),
	m_motionState(this),
	m_rigidbody(btRigidBody::btRigidBodyConstructionInfo(mass, &m_motionState, (shape && shape->Load() ? shape->GetShape() : nullptr))),
	//m_initialPosition(position),
	//m_initialOrientation(orientation)
	m_initialTransform(
		btQuaternion(orientation.x(), orientation.y(), orientation.z(), orientation.w()),
		btVector3(position.x(), position.y(), position.z()))
{

	m_rigidbody.setActivationState(DISABLE_DEACTIVATION);

}


RigidBodyComponent::~RigidBodyComponent(void)
{

}

RigidBodyComponent* RigidBodyComponent::Clone(void) const
{

	RigidBodyComponent *rb = new RigidBodyComponent(m_shape, GetMass());

	if (m_owner)
	{
		m_motionState.getWorldTransform(rb->m_initialTransform);
	}
	else
	{
		rb->m_initialTransform = m_initialTransform;
	}

	return rb;

}

void RigidBodyComponent::OnAttach(GameObject *go)
{

	Component::OnAttach(go);

	Game::GetSingleton().GetPhysicsModule()->AddRigidBody(&m_rigidbody);
	m_rigidbody.setWorldTransform(m_initialTransform);

}

void RigidBodyComponent::OnDetach(void)
{

	Game::GetSingleton().GetPhysicsModule()->RemoveRigidBody(&m_rigidbody);

	Component::OnDetach();

}

mye::math::Vector3 RigidBodyComponent::GetVelocity(void) const
{
	const btVector3 v = m_rigidbody.getLinearVelocity();
	return mye::math::Vector3(v.x(), v.y(), v.z());
}

void RigidBodyComponent::SetVelocity(const mye::math::Vector3 &v)
{
	m_rigidbody.setLinearVelocity(btVector3(v.x(), v.y(), v.z()));
}

mye::math::Vector3 RigidBodyComponent::GetPosition(void) const
{

	btTransform t;
	m_motionState.getWorldTransform(t);

	btVector3 &x = t.getOrigin();

	return mye::math::Vector3(x.x(), x.y(), x.z());

}

void RigidBodyComponent::SetPosition(const mye::math::Vector3 &x)
{
	
	btTransform t;
	m_motionState.getWorldTransform(t);
	t.setOrigin(btVector3(x.x(), x.y(), x.z()));

	m_rigidbody.setWorldTransform(t);

}

mye::math::Quaternion RigidBodyComponent::GetOrientation(void) const
{
	btTransform t;
	m_motionState.getWorldTransform(t);
	btQuaternion &x = t.getRotation();
	return mye::math::Quaternion(x.x(), x.y(), x.z(), x.w());
}

void RigidBodyComponent::SetOrientation(const mye::math::Quaternion &x)
{

	btTransform t;
	m_motionState.getWorldTransform(t);
	
	t.setRotation(btQuaternion(x.x(), x.y(), x.z(), x.w()));
	/*m_rigidbody->setCenterOfMassTransform(t);*/
	m_rigidbody.setWorldTransform(t);

}

mye::math::Real RigidBodyComponent::GetMass(void) const
{
	return mye::math::Real(1) / m_rigidbody.getInvMass();
}

void RigidBodyComponent::SetMass(const mye::math::Real &mass)
{
	m_rigidbody.setMassProps(mass, btVector3(0, 0, 0));
}

BulletCollisionShapePointer RigidBodyComponent::GetCollisionShape(void) const
{
	return m_shape;
}

void RigidBodyComponent::SetCollisionShape(BulletCollisionShapePointer shape)
{

	m_shape = shape;

	shape->Load();
	m_rigidbody.setCollisionShape(shape.get()->GetShape());

}

mye::math::Matrix4 RigidBodyComponent::GetWorldMatrix(void) const
{

	const btTransform  &t = m_rigidbody.getWorldTransform();
	const btVector3    &x = t.getOrigin();
	const btQuaternion &q = t.getRotation();

	mye::math::Vector3    mt(x.x(), x.y(), x.z());
	mye::math::Quaternion mq(q.w(), q.x(), q.y(), q.z());

	return RotationTranslationMatrix4(mq, mt);

}

btRigidBody* RigidBodyComponent::GetRigidBody(void)
{
	return &m_rigidbody;
}

/* MotionState updates the Transform Component (used for culling and rendering */

MotionState::MotionState(RigidBodyComponent *rb) :
	m_rigidbody(rb)
{

}

void MotionState::getWorldTransform(btTransform& world) const
{

	if (m_rigidbody->m_owner)
	{

		auto *tc = m_rigidbody->m_owner->GetTransformComponent();

		const mye::math::Vector3    &position    = tc->GetPosition();
		const mye::math::Quaternion &orientation = tc->GetOrientation();

		world.setOrigin(btVector3(
			position.x(),
			position.y(),
			position.z()));

		world.setRotation(btQuaternion(
			orientation.x(),
			orientation.y(),
			orientation.z(),
			orientation.w()));

	}

}

void MotionState::setWorldTransform(const btTransform& world)
{

	if (m_rigidbody->m_owner)
	{

		auto *tc = m_rigidbody->m_owner->GetTransformComponent();

		const btVector3    &btPosition = world.getOrigin();
		const btQuaternion &btOrientation = world.getRotation();

		tc->SetPosition(mye::math::Vector3(btPosition.x(), btPosition.y(), btPosition.z()));
		tc->SetOrientation(mye::math::Quaternion(btOrientation.w(), btOrientation.x(), btOrientation.y(), btOrientation.z()));

	}

}