#pragma once

#include "Control.h"
#include "IDGenerator.h"
#include <Windows.h>

#include <functional>
#include <string>

#include <mye/math/Math.h>

namespace mye
{

	namespace win
	{

		class Window;

		class Button :
			public Control
		{

		public:

			typedef std::function<void(void)> CallbackType;

			Button(void);
			~Button(void);

			void Create(Window &parent,
				const std::string &text,
				const CallbackType &f,
				const mye::math::Vector2i &position = mye::math::Vector2i(0, 0),
				const mye::math::Vector2i &size = mye::math::Vector2i(100, 100));

			void Destroy(void);

			void Show(void);
			void Hide(void);

			void SetPosition(const mye::math::Vector2i &position);
			void SetSize(const mye::math::Vector2i &size);

			std::string GetText(void) const;

			IDGenerator::ID GetID(void) const;

		private:

			friend class Window;

			HWND m_hButton;
			CallbackType m_function;

		};

	}

}

