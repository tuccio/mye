#pragma once

#include <mye/core/ColorRGBA.h>
#include <mye/core/GraphicsModule.h>

#include "DX11Device.h"
#include "DX11Window.h"

namespace mye
{

	namespace dx11
	{

		class DX11Module :
			public mye::core::GraphicsModule
		{

		public:

			DX11Module(void);
			DX11Module(DX11Window *window);

			~DX11Module(void);

			bool Init(void);
			void ShutDown(void);

			void Render(void);

			void NewWindow(void);
			void SetWindow(DX11Window *window);
			void FreeWindow(void);

		private:

			DX11Device *m_device;
			DX11Window *m_window;
			bool m_ownedDevice;

		};

	}

}
