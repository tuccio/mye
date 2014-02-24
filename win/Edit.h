#pragma once

#include "Control.h"
#include "Window.h"

#include <mye/core/String.h>

namespace mye
{

	namespace win
	{

		class Edit :
			public Control
		{

		public:

			Edit(void);
			~Edit(void);

			void Create(Window &parent,
				const mye::math::Vector2i &position = mye::math::Vector2i(0, 0),
				const mye::math::Vector2i &size = mye::math::Vector2i(120, 18));

			void Destroy(void);

			void SetText(const mye::core::String &text);
			mye::core::String GetText(void) const;

			void SetReadOnly(bool readOnly);

			void SetBorder(bool border);;

			void SetPosition(const mye::math::Vector2i &position);
			void SetSize(const mye::math::Vector2i &size);

		private:

			HWND m_hEdit;

		};

	}

}
