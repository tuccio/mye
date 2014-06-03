#include "GameObjectsModule.h"
#include "Game.h"

using namespace mye::core;
using namespace mye::math;

void GameObjectsModule::PostDestroy(GameObjectHandle hObj)
{
	m_destructionQueue.push_back(hObj);
	Get(hObj)->m_delendum = true;
}

void GameObjectsModule::Update(void)
{

	for (Iterator it = begin(); it != end(); ++it)
	{

		GameObject *object = Get(*it);

		RenderComponent    *rc = object->GetRenderComponent();
		TransformComponent *tc = object->GetTransformComponent();

		object->GetTransformComponent()->Preupdate();
		object->Update();

		Matrix4 oldTransform;

		if (object->GetTransformComponent()->Postupdate(oldTransform))
		{
			Game::GetSingleton().GetSceneModule()->
				MoveGameObject(*it, rc->GetBounds().TransformAffine(oldTransform));
		}
		
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