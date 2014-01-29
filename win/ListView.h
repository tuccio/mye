#pragma once

#include "Control.h"
#include "Window.h"

#include <mye/math/Math.h>

#include <functional>

#include <CommCtrl.h>

namespace mye
{

	namespace win
	{

		class ListViewWCR :
			public mye::core::Singleton<ListViewWCR>
		{

		public:

			ListViewWCR(void);
			~ListViewWCR(void);

			bool IsRegistered(void) const;

		private:

			bool m_registered;

		};

		class ListView :
			public Control
		{

		public:

			class Listener;

			ListView(void);
			~ListView(void);

			void Create(Window &parent,
				const mye::math::Vector2i &position = mye::math::Vector2i(0, 0),
				const mye::math::Vector2i &size = mye::math::Vector2i(100, 100));

			void Destroy(void);

			void SetPosition(const mye::math::Vector2i &position);
			void SetSize(const mye::math::Vector2i &size);

			bool Notify(unsigned int code, LPARAM lParam);

			void AddListener(Listener *listener);
			void RemoveListener(Listener *listner);
			void ClearListeners(void);

			HWND GetHandle(void);

		private:

			HWND m_hList;
			std::vector<ListView::Listener*> m_listeners;

		};

		class ListView::Listener
		{

		public:

			virtual void OnListViewEvent(unsigned int code, LPARAM lParam) = 0;

		};

	}

}
