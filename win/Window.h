#pragma once

#include <mye/core/IWindow.h>
#include <mye/core/Singleton.h>

#include <mye/math/Math.h>
#include <Windows.h>

#include "WindowMenu.h"

#include "Control.h"
#include "Tabs.h"

#include <vector>
#include <unordered_map>

#undef IsMaximized
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
			virtual bool CreateChild(HWND parent, const Properties &p = IWindow::DefaultWindowProperties);

			virtual void Destroy(void);

			virtual bool DispatchCommand(unsigned int id, unsigned int code);

			virtual bool Exists(void) const;
			virtual bool IsVisible(void) const;

			virtual void Maximize(void);
			virtual bool IsMaximized(void) const;

			virtual void Minimize(void);
			virtual bool IsMinimized(void) const;

			virtual void Show(void);
			virtual void Hide(void);

			virtual bool IsFullScreen(void) const;
			virtual void SetFullScreen(void);

			virtual void SetCaption(const mye::core::String &caption);
			virtual mye::core::String GetCaption(void) const;

			virtual void SetSize(const mye::math::Vector2i &size);
			virtual mye::math::Vector2i GetSize(void) const;

			virtual void SetPosition(const mye::math::Vector2i &position);
			virtual mye::math::Vector2i GetPosition(void) const;

			void AttachMenu(WindowMenu *menu);

			void AddMenuListener(WindowMenu::Listener *listener);
			void RemoveMenuListener(WindowMenu::Listener *listener);
			void ClearMenuListeners(void);

			void AttachTabs(Tabs *tabs);

			void AddTabsListener(Tabs::Listener *listener);
			void RemoveTabsListener(Tabs::Listener *listener);
			void ClearTabsListeners(void);

			void RegisterControl(Control *control);
			Control* FindControl(IDGenerator::ID id);
			void UnregisterControl(Control *control);
			void ClearControls(void);

			void SendUserMessage(unsigned int message);

			HWND GetHandle(void);

			HDC GetDC(void);
			bool ReleaseDC(HDC hDC);

			void Update(void);

		protected:

			virtual void _CreateMenu(WindowMenu *menu, HMENU hMenu);

			virtual bool _Create(DWORD dwExStyle,
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

			friend class WCR;
			static LRESULT CALLBACK WindowProc(HWND hWnd,
				UINT uMsg,
				WPARAM wParam,
				LPARAM lParam);

			void NotifyMenuSelected(IDGenerator::ID id);

			void NotifyShowTab(int index);
			void NotifyHideTab(int index);

			HWND m_hWnd;

			WindowMenu *m_menu;
			Tabs *m_tabs;
			std::unordered_map<IDGenerator::ID, Control*> m_controls;

			std::vector<WindowMenu::Listener*> m_menuListeners;
			std::vector<Tabs::Listener*> m_tabsListeners;

		};

		class SendQuitOnClose :
			public mye::core::IWindow::Listener
		{

		public:

			void OnDestroy(mye::core::IWindow *window);			

		};

	}
}
