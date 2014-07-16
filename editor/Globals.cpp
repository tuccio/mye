#include "Globals.h"

using namespace mye;

win::IDGenerator g_idGenerator;

SimpleLayoutWindow g_mainWindow;
win::WindowMenu g_mainWindowMenu;

int g_leftToolbarWidth = 40;

win::Window                g_renderWindow;

dx11::DX11SwapChain        g_swapChain;
mye::dx11::DX11DepthBuffer g_depthBuffer;

SceneView g_sceneView;
ModelView g_modelView;

mye::core::ResourceTypeManager g_resourceTypeManager;

mye::core::String g_currentSolution;

mye::win::MouseKeyboardInput g_input;
mye::core::GameObjectsModule g_gameObjectsModule;
mye::core::OctreeSceneModule g_scene(mye::math::Vector3f(0), 4096, 16, 2);
mye::dx11::DX11Module        g_dx11graphics;
mye::core::PhysicsModule     g_physics;
mye::core::AudioModule       g_audio;
mye::lua::LuaModule          g_lua("./scripts/");

mye::core::Game g_game(&g_input,
					   &g_gameObjectsModule,
					   &g_scene,
					   &g_physics,
					   &g_dx11graphics,
					   &g_audio,
					   &g_lua);