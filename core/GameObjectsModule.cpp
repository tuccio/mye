#include "GameObjectsModule.h"

using namespace mye::core;

void GameObjectsModule::PostDestroy(GameObjectHandle hObj)
{
	m_destructionQueue.push_back(hObj);
	Get(hObj)->m_delendum = true;
}

void GameObjectsModule::Update(FloatSeconds dt)
{

	for (Iterator it = begin(); it != end(); it++)
	{
		Get(*it)->Update(dt);
	}

// 	for (GameObjectHandle hObj : *this)
// 	{
// 		Get(hObj)->Update(dt);
// 	}

}

void GameObjectsModule::FinalizeUpdate(void)
{

	for (GameObjectHandle hObj : m_destructionQueue)
	{
		Destroy(hObj);
	}

	m_destructionQueue.clear();

}