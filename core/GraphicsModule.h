#pragma once

#include "Module.h"
#include "IWindow.h"
#include "ColorRGBA.h"

namespace mye
{

	namespace core
	{

		class GraphicsModule :
			public Module
		{

		public:

			GraphicsModule(void);

			inline bool HasWindow(void) const;

			inline IWindow* GetWindow(void);
			inline const IWindow* GetWindow(void) const;

			virtual void Render(void) = 0;

			const ColorRGBA& GetClearColor(void) const;
			void SetClearColor(const ColorRGBA &color);

		protected:

			IWindow *m_mainWindowPointer;
			ColorRGBA m_clearColor;

		};

	}

}

#include "GraphicsModule.inl"