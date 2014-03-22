#pragma once

#include <mye/math/Geometry.h>

#include <set>

namespace mye
{

	namespace core
	{

		enum class CollisionResult
		{
			ENTER,
			EXIT,
			CONTINUE,
			NONE
		};

		class Collider
		{

		public:

			Collider(const mye::math::Volume<float> &volume);
			Collider(void);

			~Collider(void);

			CollisionResult Collides(Collider *collider);

		private:

			void OnCollisionEnter(Collider *collider);
			void OnCollisionExit(Collider *collider);

			mye::math::Volume<float> *m_volume;
			std::set<Collider*> m_collidingObjects;

		};

	}

}

