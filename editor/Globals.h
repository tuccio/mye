#pragma once

#include <mye/win/Toolbar.h>
#include <mye/win/Window.h>
#include <mye/win/WindowMenu.h>

#include <mye/d3d11/DX11Window.h>
#include <mye/d3d11/DX11ShaderManager.h>

#include "View.h"
#include "SceneView.h"
#include "ModelView.h"

#include "Solution.h"
#include "SimpleLayoutWindow.h"

extern mye::win::IDGenerator g_idGenerator;

extern SimpleLayoutWindow g_mainWindow;
extern mye::win::WindowMenu g_mainWindowMenu;

extern int g_leftToolbarWidth;

extern mye::dx11::DX11Device g_device;

extern SceneView g_sceneView;

extern ModelView g_modelView;

extern Solution g_currentSolution;