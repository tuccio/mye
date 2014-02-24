#include "Globals.h"

using namespace mye;

win::IDGenerator g_idGenerator;

SimpleLayoutWindow g_mainWindow;
win::WindowMenu g_mainWindowMenu;

int g_leftToolbarWidth = 40;

dx11::DX11Device g_device;
dx11::DX11Window g_renderWindow(g_device);

SceneView g_sceneView;
ModelView g_modelView;

mye::core::String g_currentSolution;

mye::win::MouseKeyboardInput g_input;
mye::core::GameObjectsModule g_gameObjectsModule;
mye::core::OctreeSceneModule g_scene(mye::math::Vector3f(0), 4096, 16, 2);
mye::dx11::DX11Module g_dx11graphics(&(g_sceneView.GetRenderWindow()));
mye::core::AudioModule g_audio;
mye::lua::LuaModule g_lua;

mye::core::Game g_game(&g_input,
					   &g_gameObjectsModule,
					   &g_scene,
					   &g_dx11graphics,
					   &g_audio,
					   &g_lua);