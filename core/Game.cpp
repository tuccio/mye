#include "Game.h"

using namespace mye::core;

Game::Game(InputModule *input,
		   GameObjectsModule *gameobjects,
		   SceneModule *scene,
		   GraphicsModule *graphics,
		   AudioModule *audio,
		   IScriptModule *script) :
_input(input),
	_gameobjects(gameobjects),
	_scene(scene),
	_graphics(graphics),
	_audio(audio),
	_script(script)
{

}

Game::~Game(void)
{
}

bool Game::Init(void)
{

#define __TRY_INIT(x) if (!(x)->Init()) return false;

	__TRY_INIT(_input)
	__TRY_INIT(_gameobjects)
	__TRY_INIT(_scene)
	__TRY_INIT(_graphics)
	__TRY_INIT(_audio)
	__TRY_INIT(_script)

#undef __TRY_INIT

	return true;

}

void Game::Run(void)
{

}

void Game::Quit(void)
{
	exit(0);
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

IScriptModule* Game::GetScriptModule(void)
{
	return _script;
}