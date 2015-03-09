#include "ScriptResourceLoader.h"
#include "Utils.h"

using namespace mye::lua;
using namespace mye::core;

ScriptResourceLoader::ScriptResourceLoader(const mye::core::String &name)
{
}


ScriptResourceLoader::~ScriptResourceLoader(void)
{
}

bool ScriptResourceLoader::Load(Resource *resource)
{
	return m_loader.Call<bool, Resource*>("Load", std::forward<Resource*>(resource));
}

void ScriptResourceLoader::Unload(Resource *resource)
{
	m_loader.Call<void, Resource*>("Unload", std::forward<Resource*>(resource));
}