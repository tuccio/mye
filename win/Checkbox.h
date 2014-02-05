#pragma once

#include "Control.h"
#include "Window.h"

namespace mye
{

	namespace win
	{

		class Checkbox :
			public Control
		{

		public:

			Checkbox(void);
			~Checkbox(void);

			void Create(Window &parent,
				const std::string &text,
				const mye::math::Vector2i &position = mye::math::Vector2i(0, 0),
				const mye::math::Vector2i &size = mye::math::Vector2i(100, 24));

			void Destroy(void);

			bool IsChecked(void) const;
			void SetCheck(bool check);

			void Show(void);
			void Hide(void);

			void SetPosition(const mye::math::Vector2i &position);
			void SetSize(const mye::math::Vector2i &size);

		private:

			HWND m_hCheckbox;

		};

	}

}

