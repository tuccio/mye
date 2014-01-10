#include "Globals.h"

using namespace mye;

win::IDGenerator g_idGenerator;

win::Window g_mainWindow;
win::WindowMenu g_mainWindowMenu;
dx11::DX11Device g_device;
dx11::DX11Window g_childWindow(g_device);

int g_leftToolbarWidth = 40;

View *g_currentView = NULL;
SceneView g_sceneView;

Solution g_currentSolution;