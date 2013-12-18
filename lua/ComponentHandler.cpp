#include "ComponentHandler.h"

using namespace mye::core;
using namespace mye::lua;

ComponentHandler::ComponentHandler(
	const GameObjectHandle &hObj,
	Components component
	) :
		_hObj(hObj),
		_component(component)
{
}

GameObjectHandle ComponentHandler::GetGameObject(void)
{
	return _hObj;
}

Components ComponentHandler::GetComponent(void)
{
	return _component;
}