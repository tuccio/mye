#include "PhysicsModule.h"

using namespace mye::core;

PhysicsModule::PhysicsModule(void) :
	m_config(nullptr),
	m_dispatcher(nullptr),
	m_cache(nullptr),
	m_solver(nullptr),
	m_world(nullptr)
{
}


PhysicsModule::~PhysicsModule(void)
{
	ShutDown();
}

bool PhysicsModule::Init(void)
{

	m_config     = new btDefaultCollisionConfiguration;
	m_dispatcher = new btCollisionDispatcher(m_config);
	m_cache      = new btDbvtBroadphase();
	m_solver     = new btSequentialImpulseConstraintSolver;
	m_world      = new btDiscreteDynamicsWorld(m_dispatcher, m_cache, m_solver, m_config);
	
	//m_world->setGravity(btVector3(0.0f, -9.81f, 0.0f));

	return true;

}

void PhysicsModule::ShutDown(void)
{

	delete m_world;
	delete m_solver;
	delete m_cache;
	delete m_dispatcher;
	delete m_config;

	m_config     = nullptr;
	m_dispatcher = nullptr;
	m_cache      = nullptr;
	m_solver     = nullptr;
	m_world      = nullptr;


}

void PhysicsModule::Update(FloatSeconds dt)
{

	m_world->stepSimulation((btScalar) dt, 10);

}

void PhysicsModule::AddRigidBody(btRigidBody *rigidbody)
{
	m_world->addRigidBody(rigidbody);
}

void PhysicsModule::RemoveRigidBody(btRigidBody *rigidbody)
{
	m_world->removeRigidBody(rigidbody);
}

mye::math::Vector3 PhysicsModule::GetGravity(void) const
{
	btVector3 g = m_world->getGravity();
	return mye::math::Vector3(g.x(), g.y(), g.z());
}

void PhysicsModule::SetGravity(const mye::math::Vector3 &g)
{
	m_world->setGravity(btVector3(g.x(), g.y(), g.z()));
}

size_t PhysicsModule::GetBodiesCount(void) const
{
	return m_world->getNumCollisionObjects();
}