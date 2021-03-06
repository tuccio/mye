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

#include "DX11DeferredLightingRenderer.h"
#include "DX11Text2DRenderer.h"
#include "DX11DebugRenderer.h"

#include <mye/core/Singleton.h>

#define __MYE_FPS_COUNTER_BUFFER_SIZE 10

namespace mye
{

	namespace dx11
	{

		class DX11Module :
			public mye::core::GraphicsModule,
			public mye::core::IWindow::Listener,
			public mye::core::Singleton<DX11Module>,
			public mye::core::IEventListener
		{

		public:

			DX11Module(void);
			DX11Module(mye::win::Window * window);

			~DX11Module(void);

			bool Init(void);
			void Shutdown(void);

			void Render(void);

			float GetFPS(void) const;

			void RenderShaderResource(DX11ShaderResource & resource, const mye::math::Vector2i & position, const mye::math::Vector2i & size, int slice = 0);
			void RenderFrustum(const mye::math::Frustum & frustum, const mye::math::Vector4 & color);

			// Window

			void SetWindow(mye::win::Window * window);
			void FreeWindow(void);

			void OnResize(mye::core::IWindow  * window, const mye::math::Vector2i & size);

			inline DX11DepthBuffer & GetDepthBuffer(void)
			{
				return m_deferredLightingRenderer.GetDepthBuffer();
			}

			void OnEvent(const mye::core::IEvent * event);

		private:

			DX11Device                     m_device;
			mye::win::Window             * m_window;
							             
			DX11SwapChain                  m_swapChain;

			bool                           m_resizeSwapChain;
			bool                           m_changeMSAA;

			// Renderers

			DX11Text2DRenderer             m_text2dRenderer;
			DX11DeferredLightingRenderer   m_deferredLightingRenderer;
			DX11DebugRenderer              m_debugRenderer;

			// FPS Counter

			mye::core::LapStopWatch        m_stopWatch;
			mye::core::Milliseconds        m_stopWatchBuffer[__MYE_FPS_COUNTER_BUFFER_SIZE];
			int                            m_stopWatchBufferHead;

			void __CreateSharedResources(void);
			void __ApplyConfigurationChanges(void);

		};

	}

}
