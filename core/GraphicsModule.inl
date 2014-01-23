namespace mye
{

	namespace core
	{

		bool GraphicsModule::HasWindow(void) const
		{
			return m_mainWindowPointer != NULL;
		}

		IWindow* GraphicsModule::GetWindow(void)
		{
			return m_mainWindowPointer;
		}

		const IWindow* GraphicsModule::GetWindow(void) const
		{
			return m_mainWindowPointer;
		}

	}

}