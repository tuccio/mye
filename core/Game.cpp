#include "Game.h"

using namespace mye::core;

GameObjectsManager Game::_goManager;

Game::Game(void)
{
}


Game::~Game(void)
{
}

GameObjectHandle Game::CreateGameObject(void)
{
	return _goManager.Create();
}

GameObjectHandle Game::CreateGameObject(const std::string &name)
{
	return _goManager.Create(name);
}

void Game::DestroyGameObject(const GameObjectHandle &hObj)
{

	GameObject *o = _goManager.Get(hObj);

	if (o)
	{
		o->Destroy();
		_goManager.Destroy(hObj);
	}
}

GameObject* Game::GetGameObject(const GameObjectHandle &hObj)
{
	return _goManager.Get(hObj);
}

GameObjectHandle Game::FindGameObject(const std::string &name)
{
	return _goManager.Find(name);
}



bool Game::IsGameObject(const GameObjectHandle &hObj)
{
	return _goManager.Get(hObj) != NULL;
}