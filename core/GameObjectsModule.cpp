#include "GameObjectsModule.h"

using namespace mye::core;

void GameObjectsModule::Update(FloatSeconds dt)
{

	for (auto &o : m_objects)
	{
		o.second->Update(dt);
	}

}