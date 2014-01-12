#pragma once

#include <mye/win/Window.h>
#include <mye/win/WindowMenu.h>

#include <mye/d3d11/DX11Window.h>
#include <mye/d3d11/DX11ShaderManager.h>

#include "View.h"
#include "SceneView.h"
#include "Solution.h"

extern mye::win::IDGenerator g_idGenerator;

extern mye::win::Window g_mainWindow;
extern mye::win::WindowMenu g_mainWindowMenu;

extern int g_leftToolbarWidth;

extern mye::dx11::DX11Device g_device;
extern mye::dx11::DX11ShaderManager g_shaderManager;
extern mye::dx11::DX11Window g_childWindow;

extern View *g_currentView;
extern SceneView g_sceneView;

extern Solution g_currentSolution;