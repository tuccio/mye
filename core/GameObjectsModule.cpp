#include "GameObjectsModule.h"

using namespace mye::core;

void GameObjectsModule::Update(FloatSeconds dt)
{

	for (Allocation a : m_objects)
	{

		if (a.object)
		{
			a.object->Update(dt);
		}

	}

}