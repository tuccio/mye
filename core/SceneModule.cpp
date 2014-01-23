#include "SceneModule.h"

using namespace mye::core;

SceneModule::SceneModule(void)
{
	m_camera = NULL;
}

SceneModule::ObjectsList SceneModule::GetVisibleObjects(void)
{
	return ObjectsList();
}