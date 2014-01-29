#include "Globals.h"

using namespace mye;

win::IDGenerator g_idGenerator;

SimpleLayoutWindow g_mainWindow;
win::WindowMenu g_mainWindowMenu;

int g_leftToolbarWidth = 40;

dx11::DX11Device g_device;

SceneView g_sceneView(g_device);
ModelView g_modelView(g_device);

Solution g_currentSolution;

mye::win::MouseKeyboardInput g_input;
mye::core::GameObjectsModule g_gameObjectsModule;