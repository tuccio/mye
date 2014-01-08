#include <mye/core/Logger.h>
#include <mye/core/ResourceManager.h>
#include <mye/core/ResourceGroupManager.h>

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

	Logger logger;

	ResourceGroupManager rgm;
	rgm.RegisterResourceManager("Text", new TextResourceManager);

	TextResourceManager &trm = TextResourceManager::GetSingleton();

	{

		TextResourceHandle text = trm.CreateResource("hi.lua");
		TextResourceHandle t1 = trm.GetResource("hi.lua");

		text->Load();
		auto str = text->Get();

		trm.FreeResource("hi.lua");
		str = text->Get();

		TextResourceHandle t2 = trm.GetResource("hi.lua");

	}

	

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