#include "BulletCollisionShape.h"

using namespace mye::core;

BulletCollisionShape::BulletCollisionShape(ResourceManager *owner,
										   const String &name,
										   ManualResourceLoader *manual) :
	Resource(owner, name, manual),
	m_shape(nullptr)
{
}


BulletCollisionShape::~BulletCollisionShape(void)
{
}

bool BulletCollisionShape::LoadImpl(void)
{

	bool loaded = false;

	enum class __ShapeType {
		BOX,
		SPHERE
	};

	struct __StaticBlock {

		typedef std::unordered_map<String, __ShapeType> MapType;

		__StaticBlock(void)
		{
			shapes["box"]    = __ShapeType::BOX;
			shapes["sphere"] = __ShapeType::SPHERE;
		}

		MapType shapes;

	};

	if (m_params.Contains("type"))
	{

		String type = m_params.GetString("type");

		static __StaticBlock staticBlock;

		auto it = staticBlock.shapes.find(type);

		switch (it->second)
		{

		case __ShapeType::BOX:

			if (m_params.Contains("halfextents"))
			{
				mye::math::Vector3 h = m_params.GetVector3<float>("halfextents");
				m_shape = new btBoxShape(btVector3(h.x(),  h.y(), h.z()));
				loaded = true;
			}

			break;

		case __ShapeType::SPHERE:


			if (m_params.Contains("radius"))
			{
				float r = m_params.GetFloat("radius");
				m_shape = new btSphereShape(r);
				loaded = true;
			}

			break;

		}

	}

	return loaded;

}

void BulletCollisionShape::UnloadImpl(void)
{
	Clear();
}

size_t BulletCollisionShape::CalculateSizeImpl(void)
{
	return sizeof(m_shape) + sizeof(this);
}

btCollisionShape* BulletCollisionShape::GetShape(void)
{
	return m_shape;
}

void BulletCollisionShape::Clear(void)
{
	if (m_shape)
	{
		delete m_shape;
		m_shape = nullptr;
	}
}

/* Manager */

BulletCollisionShapeManager::BulletCollisionShapeManager(void) :
	ResourceManager("BulletCollisionShape")
{

}

BulletCollisionShape* BulletCollisionShapeManager::CreateImpl(const String &name,
								 ManualResourceLoader *manual,
								 const Parameters &params)
{
	return new BulletCollisionShape(this, name, manual);
}

 void BulletCollisionShapeManager::FreeImpl(Resource* resource)
 {
	 static_cast<BulletCollisionShape*>(resource)->Clear();
 }