#include "Game.h"

#include <mye/core/Logger.h>

using namespace mye::core;

Game::Game(InputModule *input,
		   GameObjectsModule *gameobjects,
		   SceneModule *scene,
		   GraphicsModule *graphics,
		   AudioModule *audio,
		   IScriptModule *script) :
m_input(input),
	m_gameobjects(gameobjects),
	m_scene(scene),
	m_graphics(graphics),
	m_audio(audio),
	m_script(script)
{

}

Game::~Game(void)
{
}

bool Game::Init(void)
{

#define __TRY_INIT(x) if (!(x)->Init()) return false;

	__TRY_INIT(m_input)
	__TRY_INIT(m_gameobjects)
	__TRY_INIT(m_scene)
	__TRY_INIT(m_graphics)
	__TRY_INIT(m_audio)
	__TRY_INIT(m_script)

#undef __TRY_INIT

	return true;

}

void Game::Run(void)
{
	m_timer.Start();
}

void Game::Quit(void)
{
	m_timer.Stop();
	exit(0);
}

InputModule* Game::GetInputModule(void)
{
	return m_input;
}

GameObjectsModule* Game::GetGameObjectsModule(void)
{
	return m_gameobjects;
}

SceneModule* Game::GetSceneModule(void)
{
	return m_scene;
}

GraphicsModule* Game::GetGraphicsModule(void)
{
	return m_graphics;
}

AudioModule* Game::GetAudioModule(void)
{
	return m_audio;
}

IScriptModule* Game::GetScriptModule(void)
{
	return m_script;
}

void Game::RuntimeError(const std::string &error)
{
	mye::core::Logger::LogErrorOptional(error);
}