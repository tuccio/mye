#pragma once

#include <mye/core/IWindow.h>
#include <mye/core/Singleton.h>

#include <Eigen/Eigen>
#include <Windows.h>

#include "WindowMenu.h"

#undef IsMinimized

namespace mye
{

	namespace win
	{

		class WCR :
			public mye::core::Singleton<WCR>
		{

		public:

			WCR(void);
			~WCR(void);

			bool IsRegistered(void) const;

		private:

			bool m_registered;

		};

		class Window : public mye::core::IWindow
		{

		public:

			Window(void);

			~Window(void);

			virtual bool Create(const Properties &p = IWindow::DefaultWindowProperties);
			virtual bool CreateChild(Window &parent, const Properties &p = IWindow::DefaultWindowProperties);

			virtual void Destroy(void);

			virtual bool Exists(void) const;
			virtual bool IsVisible(void) const;

			virtual bool IsMinimized(void) const;

			virtual void Show(void);
			virtual void Hide(void);

			virtual bool IsFullScreen(void) const;
			virtual void SetFullScreen(void);

			virtual void SetCaption(const std::string &caption);
			virtual std::string GetCaption(void) const;

			virtual void SetSize(const Eigen::Vector2i &size);
			virtual Eigen::Vector2i GetSize(void) const;

			virtual void SetPosition(const Eigen::Vector2i &position);
			virtual Eigen::Vector2i GetPosition(void) const;

			void AttachMenu(WindowMenu *menu);
			void _CreateMenu(WindowMenu *menu, HMENU hMenu);

			void AddMenuListener(WindowMenu::Listener *listener);
			void RemoveMenuListener(WindowMenu::Listener *listener);
			void ClearMenuListeners(void);

			void SendUserMessage(unsigned int message);

			HWND GetHandle(void);

			HDC GetDC(void);
			bool ReleaseDC(HDC hDC);

			bool _Create(DWORD dwExStyle,
				LPCTSTR lpClassName,
				LPCTSTR lpWindowName,
				DWORD dwStyle,
				int x,
				int y,
				int nWidth,
				int nHeight,
				HWND hWndParent,
				HMENU hMenu,
				HINSTANCE hInstance,
				LPVOID lpParam);


		private:

			void NotifyMenuSelected(IDGenerator::ID id);

			HWND m_hWnd;
			WindowMenu *m_menu;

			std::vector<WindowMenu::Listener*> m_menuListeners;

			friend class WCR;
			static LRESULT CALLBACK WindowProc(HWND hWnd,
				UINT uMsg,
				WPARAM wParam,
				LPARAM lParam);

		};

	}
}
