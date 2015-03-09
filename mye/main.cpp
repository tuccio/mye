#include <mye/core/BulletCollisionShape.h>
#include <mye/core/EntityManager.h>
#include <mye/core/FontManager.h>
#include <mye/core/ImageManager.h>
#include <mye/core/MaterialManager.h>
#include <mye/core/MeshManager.h>
#include <mye/core/OctreeSceneModule.h>
#include <mye/core/PhysicsModule.h>
#include <mye/core/ResourceManager.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include <mye/d3d11/DX11Module.h>
#include <mye/d3d11/DX11ShaderManager.h>
#include <mye/d3d11/DX11TextureManager.h>
#include <mye/d3d11/DX11BufferManager.h>
#include <mye/d3d11/DX11FontManager.h>

#include <mye/lua/LuaModule.h>

#include <mye/win/MouseKeyboardInput.h>
#include <mye/win/Utils.h>
#include <mye/win/WinGame.h>

#include <iostream>
#include <Windows.h>

#include "MainWindowListener.h"

#include <mye/core/RawData.h>

using namespace mye::core;
using namespace mye::dx11;
using namespace mye::lua;
using namespace mye::win;

int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{

	mye::win::CreateConsole();

	/* Main window */

	Window window;

	/*
	 * Allocate needed managers and modules
	 */

	ResourceTypeManager         resourceTypeManager;
	EntityManager               entityTemplateManager("./entities/");
	MaterialManager             materialManager;
	MeshManager                 meshManager;
	BulletCollisionShapeManager bulletCollisionShape;
	ImageManager                imageManager;

	MouseKeyboardInput input;
	GameObjectsModule  gameobjects;
	OctreeSceneModule  scene;
	PhysicsModule      physics;
	DX11Module         graphics;
	AudioModule        audio;
	LuaModule          lua("./scripts/");	


	/*
	 * Graphics setup
	 */

	DX11Device * device = static_cast<DX11Module&>(graphics).GetDevice();

	DX11BufferManager  bufferManager(*device);
	DX11ShaderManager  shaderManager(*device);
	DX11TextureManager textureManager(*device);
	DX11FontManager    fontManager;

	/*
	 * Move working directory to the project folder
	 */

	std::vector<String> commandLine = SplitCommandLine(lpCmdLine);

	String projectName;
	String mainScriptName = "main";

	if (commandLine.size() == 1)
	{
		projectName = commandLine[0];
	}
	else
	{
		SetCurrentDirectory(commandLine[0].CString());
		projectName = commandLine[1];

		if (commandLine.size() > 2)
		{
			mainScriptName = commandLine[2];
		}

	}

	/*
	 * Create and run the game
	 */

	MaterialPointer mat = ResourceTypeManager::GetSingleton().CreateResource<Material>("Material", "./materials/white.mym");
	mat->Load();

	WinGame game(&input,
				 &gameobjects,
				 &scene,
				 &physics,
				 &graphics,
				 &audio,
				 &lua);

	/*
	 * Setup our window
	 */

	if (!window.Create())
	{
		ShowErrorBox("Window creation failed.");
		return 1;
	}

	graphics.SetWindow(&window);

	MainWindowListener mainWindowListener;
	window.AddListener(&mainWindowListener);

	if (!game.Init())
	{
		ShowErrorBox("Initialization failed.");
		return 1;
	}

	if (!projectName.IsEmpty())
	{
		game.ImportScene(projectName);
	}

	ProcedureScriptPointer main = lua.LoadProcedure(mainScriptName);

	if (main->Load())
	{
		main->Run();
		main->Unload();
	}
	else if (!main->GetLastError().IsEmpty())
	{
		ShowErrorBox(mainScriptName + ": " + main->GetLastError());
	}

	game.Run();

	return 0;

}