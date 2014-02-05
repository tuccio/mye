#pragma once

#include <mye/core/Game.h>
#include <mye/core/OctreeSceneModule.h>

#include <mye/win/Toolbar.h>
#include <mye/win/MouseKeyboardInput.h>
#include <mye/win/Window.h>
#include <mye/win/WindowMenu.h>

#include <mye/d3d11/DX11Module.h>
#include <mye/d3d11/DX11ShaderManager.h>
#include <mye/d3d11/DX11Window.h>

#include <mye/lua/LuaModule.h>

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
extern mye::dx11::DX11Window g_renderWindow;

extern SceneView g_sceneView;
extern ModelView g_modelView;

extern Solution g_currentSolution;

extern mye::win::MouseKeyboardInput g_input;
extern mye::core::GameObjectsModule g_gameObjectsModule;
extern mye::core::OctreeSceneModule g_scene;
extern mye::dx11::DX11Module g_dx11graphics;
extern mye::core::AudioModule g_audio;
extern mye::lua::LuaModule g_lua;

extern mye::core::Game g_game;

// extern mye::win::WinKeyboard g_keyboard;
//extern mye::win::WinMouse g_mouse;