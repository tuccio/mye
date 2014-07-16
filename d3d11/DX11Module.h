#pragma once

#include <mye/core/AlignedAllocator.h>
#include <mye/core/GraphicsModule.h>
#include <mye/core/Time.h>

#include <mye/win/Window.h>

#include "DX11Device.h"
#include "DX11DepthBuffer.h"
#include "DX11SwapChain.h"

#include "DX11ConstantBuffer.h"
#include "DX11Texture.h"

#include "DX11PixelShader.h"
#include "DX11VertexShader.h"

#include "BasicRenderer.h"
#include "DeferredLightingRenderer.h"
#include "Text2DRenderer.h"

#define __MYE_FPS_COUNTER_BUFFER_SIZE 10

namespace mye
{

	namespace dx11
	{

		class DX11Module :
			public mye::core::GraphicsModule,
			public mye::core::IWindow::Listener
		{

		public:

			DX11Module(void);
			DX11Module(mye::win::Window *window);

			~DX11Module(void);

			bool Init(void);
			void Shutdown(void);

			void Render(void);

			float GetFPS(void) const;

			inline DX11Device* GetDevice(void);

			// Window

			void SetWindow(mye::win::Window *window);
			void FreeWindow(void);

			void OnResize(mye::core::IWindow *window, const mye::math::Vector2i &size);
			

		private:

			DX11Device        m_device;
			mye::win::Window *m_window;

			DX11SwapChain     m_swapChain;

			// Renderers

			Text2DRenderer           m_text2dRenderer;
			BasicRenderer            m_basicRenderer;
			DeferredLightingRenderer m_deferredLighthingRenderer;

			// FPS Counter

			mye::core::LapStopWatch m_stopWatch;
			mye::core::Milliseconds m_stopWatchBuffer[__MYE_FPS_COUNTER_BUFFER_SIZE];
			int                     m_stopWatchBufferHead;

		};

		DX11Device* DX11Module::GetDevice(void)
		{
			return &m_device;
		}

	}

}
