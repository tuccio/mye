#include "Game.h"

using namespace mye::core;

Game::Game(InputModule *input,
		   GameObjectsModule *gameobjects,
		   SceneModule *scene,
		   GraphicsModule *graphics,
		   AudioModule *audio) :
	_input(input),
	_gameobjects(gameobjects),
	_scene(scene),
	_graphics(graphics),
	_audio(audio)
{
}


Game::~Game(void)
{
}

void Game::Run(void)
{

}

InputModule* Game::GetInputModule(void)
{
	return _input;
}

GameObjectsModule* Game::GetGameObjectsModule(void)
{
	return _gameobjects;
}

SceneModule* Game::GetSceneModule(void)
{
	return _scene;
}

GraphicsModule* Game::GetGraphicsModule(void)
{
	return _graphics;
}

AudioModule* Game::GetAudioModule(void)
{
	return _audio;
}