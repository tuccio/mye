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

			const mye::math::Vector4f& GetClearColor(void) const;
			void SetClearColor(const mye::math::Vector4f &color);

		protected:

			IWindow            *m_mainWindowPointer;
			mye::math::Vector4f m_clearColor;

		};

	}

}

#include "GraphicsModule.inl"