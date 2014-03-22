#include "Collider.h"

using namespace mye::core;
using namespace mye::math;

Collider::Collider(const Volume<float> &volume) :
	m_volume(volume.Clone())
{
}

Collider::Collider(void) :
	m_volume(nullptr)
{
}


Collider::~Collider(void)
{

	if (m_volume)
	{
		delete m_volume;
	}

}

CollisionResult Collider::Collides(Collider *collider)
{
	
	CollisionResult r = CollisionResult::NONE;

	if (collider->m_volume && m_volume)
	{

		VolumeSide side = collider->m_volume->Intersect(*m_volume);

		switch (side)
		{

		case VolumeSide::INSIDE:
		case VolumeSide::INTERSECT:

			if (m_collidingObjects.find(collider) != m_collidingObjects.end())
			{
				r = CollisionResult::CONTINUE;
			}
			else
			{
				r = CollisionResult::ENTER;
				m_collidingObjects.insert(collider);
				OnCollisionEnter(collider);
				//collider->OnCollisionEnter(this);
			}

			break;

		case VolumeSide::OUTSIDE:

			auto it = m_collidingObjects.find(collider);

			if (it != m_collidingObjects.end())
			{
				r = CollisionResult::EXIT;
				m_collidingObjects.erase(it);
				OnCollisionExit(collider);
			}

			break;

		}

	}

	return r;

}