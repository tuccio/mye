namespace mye
{

	namespace core
	{

		mye::lua::LuaScript& ScriptComponent::Script(void)
		{
			return m_script;
		}

		inline const mye::lua::LuaScript& ScriptComponent::Script(void) const
		{
			return m_script;
		}

	}

}