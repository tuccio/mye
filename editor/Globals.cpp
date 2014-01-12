#include "Globals.h"

using namespace mye;

win::IDGenerator g_idGenerator;

win::Window g_mainWindow;
win::WindowMenu g_mainWindowMenu;

int g_leftToolbarWidth = 40;

dx11::DX11Device g_device;
dx11::DX11Window g_childWindow(g_device);
dx11::DX11ShaderManager g_shaderManager(g_device);

View *g_currentView = NULL;
SceneView g_sceneView;

Solution g_currentSolution;