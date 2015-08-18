#include "GameObjectsModule.h"
#include "Game.h"

using namespace mye::core;
using namespace mye::math;

bool GameObjectsModule::Init(void)
{
	MYE_EVENT_MANAGER_ADD_LISTENER(this, EventType::GAME_OBJECT_DESTROY);
	return true;
}

void GameObjectsModule::Shutdown(void)
{
	MYE_EVENT_MANAGER_REMOVE_LISTENER(this, EventType::GAME_OBJECT_DESTROY);
}

void GameObjectsModule::PostDestroy(GameObjectHandle hObj)
{

	MYE_EVENT_MANAGER_ENQUEUE(GameObjectEventDestroy, hObj);

}

void GameObjectsModule::Update(void)
{

	/*for (Iterator it = begin(); it != end(); ++it)
	{

		GameObject *object = Get(*it);

		RenderComponent    *rc = object->GetRenderComponent();
		TransformComponent *tc = object->GetTransformComponent();

		object->GetTransformComponent()->Preupdate();
		object->Update();

		Matrix4 oldTransform;

		if (rc && object->GetTransformComponent()->Postupdate(oldTransform))
		{
			Game::GetSingleton().GetSceneModule()->
				MoveGameObject(*it, rc->GetBounds().TransformAffine(oldTransform));
		}
		
	}*/

// 	for (GameObjectHandle hObj : *this)
// 	{
// 		Get(hObj)->Update(dt);
// 	}

}

void GameObjectsModule::Preupdate(void)
{

	for (GameObjectHandle hObj : m_destructionQueue)
	{
		Destroy(hObj);
	}

	m_destructionQueue.clear();

}

void GameObjectsModule::OnEvent(const IEvent * e)
{

	switch (e->event)
	{

	case EventType::GAME_OBJECT_DESTROY:
	{
		const GameObjectEventDestroy * goEvent = static_cast<const GameObjectEventDestroy *>(e);
		m_destructionQueue.push_back(goEvent->hObj);
		//MYE_EVENT_MANAGER_ENQUEUE_NEXT_FRAME(GameObjectEventFree, goEvent->object);
		break;
	}

	/*case EventType::GAME_OBJECT_FREE:
	{
		const GameObjectEventFree * goEvent = static_cast<const GameObjectEventFree *>(e);
		Destroy(goEvent->object->GetHandle());
		break;
	}*/

	}

}