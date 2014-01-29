#pragma once

#include "Mouse.h"
#include "Keyboard.h"

#include <string>
#include <vector>

#include <mye/math/Math.h>

namespace mye
{

	namespace core
	{

		class IWindow
		{

		public:

			class Listener;

			struct Properties;

			static const Properties DefaultWindowProperties;

			IWindow(void);
			virtual ~IWindow(void) = 0;

			virtual bool Create(const Properties &p = DefaultWindowProperties) = 0;
			virtual void Destroy(void) = 0;

			virtual bool Exists(void) const = 0;
			virtual bool IsVisible(void) const = 0;

			virtual bool DispatchCommand(unsigned int id, unsigned int code) = 0;

			virtual void Maximize(void) = 0;
			virtual bool IsMaximized(void) const = 0;

			virtual void Minimize(void) = 0;
			virtual bool IsMinimized(void) const = 0;

			virtual void Show(void) = 0;
			virtual void Hide(void) = 0;

			virtual bool IsFullScreen(void) const = 0;
			virtual void SetFullScreen(void) = 0;

			virtual void SetCaption(const std::string &caption) = 0;
			virtual std::string GetCaption(void) const = 0;

			virtual void SetSize(const mye::math::Vector2i &size) = 0;
			virtual mye::math::Vector2i GetSize(void) const = 0;

			virtual void SetPosition(const mye::math::Vector2i &position) = 0;
			virtual mye::math::Vector2i GetPosition(void) const = 0;

			void AttachKeyboard(mye::core::Keyboard *keyboard);
			void AttachMouse(mye::core::Mouse *mouse);

			void SendUserMessage(unsigned int msg,
				unsigned int intArg,
				long *ptrArg);

			void AddListener(Listener *listener);
			void RemoveListener(Listener *listener);
			void ClearListeners(void);

		protected:

			void NotifyCreate(void);
			void NotifyDestroy(void);
			void NotifyResize(const mye::math::Vector2i &size);
			void NotifyCommand(unsigned int id, unsigned int code);
			void NotifyUserMessage(unsigned int msg,
				unsigned long intArg,
				long *ptrArg);

		protected:

			typedef std::vector<Listener*> ListenerList;

			ListenerList m_listeners;

			mye::core::Keyboard *m_keyboard;
			mye::core::Mouse *m_mouse;

		};

		struct IWindow::Properties
		{

			std::string caption;

			int x;
			int y;

			int width;
			int height;

			bool fullscreen;

		};

		class IWindow::Listener
		{

		public:

			virtual inline void OnCreate(IWindow *window) { }
			virtual inline void OnDestroy(IWindow *window) { }
			virtual inline void OnResize(IWindow *window, const mye::math::Vector2i &size) { }
			virtual inline void OnCommand(unsigned int id, unsigned int code) { }
			virtual inline void OnUserMessage(IWindow *window,
				unsigned int msg,
				unsigned long intArg,
				long *ptrArg) { }

		};

	}

}

