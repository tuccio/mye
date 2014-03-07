#include <mye/core/Logger.h>
#include <mye/core/Entity.h>
#include <mye/core/ResourceManager.h>
#include <mye/core/ResourceTypeManager.h>

#include <mye/d3d11/DX11Module.h>

#include <mye/lua/LuaModule.h>

#include <mye/win/MouseKeyboardInput.h>
#include <mye/win/WinGame.h>

#include <iostream>
#include <Windows.h>

using namespace std;
using namespace mye::core;
using namespace mye::dx11;
using namespace mye::lua;
using namespace mye::win;

int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{

	ResourceTypeManager resourceTypeManager;
	EntityTemplateManager entityTemplateManager("./entity/");

	MouseKeyboardInput input;
	GameObjectsModule gameobjects;
	SceneModule scene;
	DX11Module graphics;
	AudioModule audio;
	LuaModule lua("./scripts/");

	WinGame game(&input,
		&gameobjects,
		&scene,
		&graphics,
		&audio,
		&lua);

	if (!game.Init())
	{
		MessageBox(nullptr, "Initialization failed", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Load classes
	
	// Run the main procedure and the game

	lua.LoadProcedure("main")->Run();
	game.Run();
	
	return 0;

}