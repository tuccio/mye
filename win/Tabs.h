#pragma once

#include <Windows.h>

#include <mye/core/Singleton.h>
#include <mye/math/Math.h>

#include <string>

namespace mye
{

	namespace win
	{

		class TabsWCR :
			public mye::core::Singleton<TabsWCR>
		{

		public:

			TabsWCR(void);
			~TabsWCR(void);

			bool IsRegistered(void) const;

		private:

			bool m_registered;

		};

		class Window;

		class Tabs
		{

		public:

			class Listener;

			Tabs(Window &window);
			~Tabs(void);

			void Create(void);
			void Destroy(void);

			void Show(void);
			void Hide(void);

			void SetPosition(const mye::math::Vector2i &position);
			void SetSize(const mye::math::Vector2i &size);
			
			void SelectTab(int index);
			int GetSelectedTab(void);

			HWND GetHandle(void);

			void AddTab(int index,
				const std::string &label);

		private:

			HWND m_hTabs;
			Window &m_parent;

		};

		class Tabs::Listener
		{

		public:

			virtual void OnTabHide(int index);
			virtual void OnTabShow(int index);

		};

	}

}
