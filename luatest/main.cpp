#include <mye/core/Logger.h>
#include <mye/core/ResourceManager.h>

#include <mye/lua/LuaScriptCaller.h>
#include <mye/lua/LuaScript.h>
#include <mye/lua/LuaModule.h>

#include <mye/win/WinGame.h>

#include <iostream>

#include "TextResource.h"

#include <Windows.h>

using namespace std;
using namespace mye::core;
using namespace mye::lua;
using namespace mye::win;

int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{

	Logger logger;

	logger.OpenErrorLogFile("error.log");
	logger.OpenEventLogFile("event.log");

	InputModule input;
	GameObjectsModule gameobjects;
	SceneModule scene;
	GraphicsModule graphics;
	AudioModule audio;
	LuaModule lua;

	WinGame game(&input,
		&gameobjects,
		&scene,
		&graphics,
		&audio,
		&lua);

	if (!game.Init())
	{
		MessageBox(NULL, "Initialization failed", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Load classes
	
	// Run the main procedure and the game

	lua.LoadProcedure("main.lua").Run();
	game.Run();
	
	return 0;

}