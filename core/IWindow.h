#pragma once

#include <string>
#include <Eigen/Eigen>

namespace mye
{

	namespace core
	{

		class IWindow
		{

		public:

			struct Properties;

			IWindow(void);
			virtual ~IWindow(void) = 0;

			virtual bool Create(void) = 0;
			virtual bool Create(const Properties &p) = 0;
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

		};

		struct IWindow::Properties
		{

			std::string name;

			int x;
			int y;

			int width;
			int height;

			bool fullscreen;

		};

	}

}

