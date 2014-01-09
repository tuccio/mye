namespace mye
{

	namespace core
	{

		template <typename R>
		R Script<mye::lua::LuaScriptCaller>::Call(const std::string &f) const
		{
			return mye::lua::LuaScriptCaller::Call<R>(m_module, *this, f);
		}

	}

}