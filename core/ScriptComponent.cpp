#include "ScriptComponent.h"

using namespace mye::core;
using namespace mye::lua;

ScriptComponent::ScriptComponent(LuaScript script) :
	Component(COMPONENT_SCRIPT, "script"),
	m_script(script)
{
}


ScriptComponent::~ScriptComponent(void)
{
}

Cloneable* ScriptComponent::Clone(void) const
{
	return new ScriptComponent(m_script);
}
