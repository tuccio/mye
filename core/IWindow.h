#pragma once

#include <string>
#include <vector>
#include <Eigen/Eigen>

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

			virtual bool IsMinimized(void) const = 0;

			virtual void Show(void) = 0;
			virtual void Hide(void) = 0;

			virtual bool IsFullScreen(void) const = 0;
			virtual void SetFullScreen(void) = 0;

			virtual void SetCaption(const std::string &caption) = 0;
			virtual std::string GetCaption(void) const = 0;

			virtual void SetSize(const Eigen::Vector2i &size) = 0;
			virtual Eigen::Vector2i GetSize(void) const = 0;

			virtual void SetPosition(const Eigen::Vector2i &position) = 0;
			virtual Eigen::Vector2i GetPosition(void) const = 0;

			void AddListener(Listener *listener);
			void RemoveListener(Listener *listener);
			void ClearListeners(void);

		protected:

			void NotifyCreate(void);
			void NotifyDestroy(void);
			void NotifyResize(const Eigen::Vector2i &size);

		private:

			typedef std::vector<Listener*> ListenerList;

			ListenerList m_listeners;

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
			virtual inline void OnDestroy(IWindow * window) { }
			virtual inline void OnResize(IWindow *window, const Eigen::Vector2i &size) { }

		};

	}

}

