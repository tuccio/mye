#pragma once

#include "Window.h"

#include <Commctrl.h>

namespace mye
{

	namespace win
	{

		class ToolbarWCR :
			public mye::core::Singleton<ToolbarWCR>
		{

		public:

			ToolbarWCR(void);
			~ToolbarWCR(void);

			bool IsRegistered(void) const;

		private:

			bool m_registered;

		};

		class Toolbar
		{

		public:

			Toolbar(Window &window,
				bool vertical = false);
			~Toolbar(void);

		private:

			HWND m_hWndToolbar;
			HIMAGELIST m_imageList;
			bool m_vertical;

			friend class ToolbarWCR;
			static LRESULT CALLBACK WindowProc(HWND hWnd,
				UINT uMsg,
				WPARAM wParam,
				LPARAM lParam);

		};

	}

}
