#include "ScriptResourceLoader.h"

using namespace mye::lua;

ScriptResourceLoader::ScriptResourceLoader(LuaModule &luaModule,
										   const mye::core::String &name) :
	BehaviourScript(luaModule, name)
{
}


ScriptResourceLoader::~ScriptResourceLoader(void)
{
}

bool ScriptResourceLoader::Load(bool background)
{
	return BehaviourScript::Load(background);
}

void ScriptResourceLoader::Unload(bool background)
{
	BehaviourScript::Unload(background);
}

bool ScriptResourceLoader::Load(Resource *resource)
{
	return Call<bool, Resource*>("Load", resource);
}

void ScriptResourceLoader::Unload(Resource *resource)
{
	Call<void, Resource*>("Unload", resource);
}