#include <mye/core/Entity.h>
#include <mye/core/ModelManager.h>
#include <mye/core/OctreeSceneModule.h>
#include <mye/core/ResourceManager.h>
#include <mye/core/ResourceTypeManager.h>

#include <mye/d3d11/DX11Module.h>
#include <mye/d3d11/DX11ShaderManager.h>

#include <mye/lua/LuaModule.h>

#include <mye/win/MouseKeyboardInput.h>
#include <mye/win/Utils.h>
#include <mye/win/WinGame.h>

#include <iostream>
#include <regex>
#include <Windows.h>

#include "MainWindowListener.h"

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
	
	/*
	 * Allocate needed managers and modules
	 */

	ResourceTypeManager resourceTypeManager;
	EntityTemplateManager entityTemplateManager("./entities/");
	ModelManager modelManager;

	MouseKeyboardInput input;
	GameObjectsModule gameobjects;
	OctreeSceneModule scene;
	DX11Module graphics;
	AudioModule audio;
	LuaModule lua("./scripts/");

	/*
	 * Prepare shaders
	 */

	DX11ShaderManager shaderManager(static_cast<DX11Window*>(graphics.GetWindow())->GetDevice());

	std::vector<D3D11_INPUT_ELEMENT_DESC> vDesc(1);

	vDesc[0].SemanticName         = "POSITION";
	vDesc[0].SemanticIndex        = 0;	
	vDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	vDesc[0].InputSlot            = 0;
	vDesc[0].AlignedByteOffset    = 0;
	vDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	vDesc[0].InstanceDataStepRate = 0;

	shaderManager.CreateVertexShader("VertexShader.cso", true, vDesc)->Use();
	shaderManager.CreatePixelShader("PixelShader.cso", true)->Use();

	/*
	 * Move working directory to the project folder
	 */

	std::vector<String> commandLine = SplitCommandLine(lpCmdLine);

	String projectName;

	if (commandLine.size() == 1)
	{
		projectName = commandLine[0];
	}
	else if (commandLine.size() == 2)
	{
		SetCurrentDirectory(commandLine[0].CString());
		projectName = commandLine[1];
	}

	/*
	 * Create and run the game
	 */

	WinGame game(&input,
		&gameobjects,
		&scene,
		&graphics,
		&audio,
		&lua);

	MainWindowListener mainWindowListener(&game);
	graphics.GetWindow()->AddListener(&mainWindowListener);

	if (!game.Init())
	{
		ShowErrorBox("Initialization failed");
		return 1;
	}

	if (!projectName.IsEmpty())
	{
		game.ImportScene(projectName);
	}

	ProcedureScriptPointer main = lua.LoadProcedure("main");

	if (main->GetState() == LoadingState::LOADED)
	{
		main->Run();
		main->Unload();
	}
	
	game.Run();

	return 0;

}