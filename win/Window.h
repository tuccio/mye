#pragma once

#include <mye/core/IWindow.h>

#include <Windows.h>
#include <Eigen/Eigen>
#include <vector>

namespace mye
{

	namespace win
	{

		class Window : public mye::core::IWindow
		{

		public:

			Window(void);
			~Window(void);

			bool Create(void);
			bool Create(const Properties &p);
			void Destroy(void);

			bool Exists(void) const;
			bool IsVisible(void) const;

			bool IsMinimized(void) const;

			void Show(void);
			void Hide(void);

			bool IsFullScreen(void) const;
			void SetFullScreen(void);

			void SetCaption(const std::string &caption);
			std::string GetCaption(void) const;

			void SetSize(const Eigen::Vector2i &size);
			Eigen::Vector2i GetSize(void) const;

			void SetPosition(const Eigen::Vector2i &position);
			Eigen::Vector2i GetPosition(void) const;

		private:

			HWND _hWnd;
			HDC _hDC;

			class WindowClassRegisterer;

			static const Properties _defaultWindowProperties;
			static WindowClassRegisterer _wcr;

			static LRESULT CALLBACK WindowProc(HWND hWnd,
											   UINT uMsg,
											   WPARAM wParam,
											   LPARAM lParam);

		};

		

		class Window::WindowClassRegisterer
		{

		public:

			WindowClassRegisterer(void);

			bool IsRegistered(void) const;
			void Register(void);

		private:

			bool _registered;

		};

	}

}
