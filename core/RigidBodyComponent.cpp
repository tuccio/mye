#include "RigidBodyComponent.h"

#include "Game.h"

using namespace mye::core;
using namespace mye::math;

RigidBodyComponent::RigidBodyComponent(btCollisionShape *shape,
									   mye::math::Real mass,
									   const Vector3 &position,
									   const Quaternion &orientation) :
	Component(ComponentTypes::RIGIDBODY, "rigidbody"),
	m_motionState(
		btTransform(
			btQuaternion(orientation.x(), orientation.y(), orientation.z(), orientation.w()),
			btVector3(position.x(), position.y(), position.z()))),
	m_rigidbody(mass, &m_motionState, shape),
	m_shape(shape)
{
	Game::GetSingleton().GetPhysicsModule()->AddRigidBody(&m_rigidbody);
}


RigidBodyComponent::~RigidBodyComponent(void)
{
	delete m_shape;
}