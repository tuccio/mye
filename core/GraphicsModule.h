#pragma once

#include "Module.h"
#include "IWindow.h"

#include <mye/math/Math.h>

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

			virtual float GetFPS(void) const = 0;

			const mye::math::Vector4 & GetClearColor(void) const;
			void SetClearColor(const mye::math::Vector4 &color);

			bool GetVSync(void) const;
			void SetVSync(bool vsync);

			inline RendererConfiguration * GetRendererConfiguration(void)
			{
				return &m_rendererConfiguration;
			}

		protected:

			IWindow               * m_mainWindowPointer;
			mye::math::Vector4      m_clearColor;
								     
			bool                    m_vsync;

			RendererConfiguration   m_rendererConfiguration;

		};

	}

}

#include "GraphicsModule.inl"