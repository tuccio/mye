#pragma once

#include <mye/core/IWindow.h>
#include <mye/core/Singleton.h>

#include <Eigen/Eigen>
#include <Windows.h>

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

			HWND m_hWnd;

			friend class WCR;
			static LRESULT CALLBACK WindowProc(HWND hWnd,
				UINT uMsg,
				WPARAM wParam,
				LPARAM lParam);

		};

	}
}
