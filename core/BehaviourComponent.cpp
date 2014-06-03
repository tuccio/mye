#include "BehaviourComponent.h"

#include "GameObject.h"
#include "Utils.h"
#include "Game.h"

#include <mye/lua/LuaModule.h>

#include <luabind/luabind.hpp>

using namespace mye::core;

BehaviourComponent::BehaviourComponent(void) :
	Component(ComponentTypes::BEHAVIOUR, "behaviour")
{
}

BehaviourComponent::BehaviourComponent(const mye::lua::BehaviourScriptPointer &s) :
	Component(ComponentTypes::BEHAVIOUR, "behaviour"),
	m_script(s)
{
}


BehaviourComponent::~BehaviourComponent(void)
{
}

BehaviourComponent* BehaviourComponent::Clone(void) const
{
	return new BehaviourComponent(*this);
}

void BehaviourComponent::Init(void)
{

	lua_State *L = m_script->GetLuaState();
	luabind::globals(L)["self"] = luabind::object(L, m_owner->GetHandle());

	try
	{
		m_script->Call<void>("Init");
	}
	catch (mye::lua::CallException ex)
	{

		if (ex.error == mye::lua::CallExceptionError::RUNTIME)
		{
			RuntimeError(m_script->GetName() + ".Init()\n\n" + ex.msg);
		}

	}

	luabind::globals(L)["self"] = luabind::nil;

}

void BehaviourComponent::Update(void)
{

	lua_State *L = m_script->GetLuaState();
	luabind::globals(L)["self"] = luabind::object(L, m_owner->GetHandle());

	try
	{
		m_script->Call<void>("Update");
	}
	catch (mye::lua::CallException ex)
	{

		if (ex.error == mye::lua::CallExceptionError::RUNTIME)
		{
			RuntimeError(m_script->GetName() + ".Update()\n\n" + ex.msg);
		}

	}

	luabind::globals(L)["self"] = luabind::nil;

}