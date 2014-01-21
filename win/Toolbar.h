#pragma once

#include "Window.h"

#include <Windows.h>
#include <Commctrl.h>

#include <utility>
#include <unordered_map>
#include <functional>

namespace mye
{

	namespace win
	{

		class ToolbarWCR :
			public mye::core::Singleton<ToolbarWCR>
		{

		public:

			ToolbarWCR(void);
			~ToolbarWCR(void);

			bool IsRegistered(void) const;

		private:

			bool m_registered;

		};

		class Toolbar
		{

		public:

			typedef std::function<void(void)> CallbackType;

			Toolbar(Window &window,
				bool vertical = false);
			~Toolbar(void);

			void Create(void);
			void SetVertical(bool vertical);

			void AddButton(const std::string &icon,
				const CallbackType &f,
				const std::string &text = std::string());

			bool DispatchCommand(unsigned int id);

			void ShowText(void);
			void HideText(void);

			mye::math::Vector2i GetSize(void) const;
			void AutoSize(void);

			inline bool IsVertical(void) const 
			{
				return m_vertical;
			}

			void Show(void);
			void Hide(void);

			mye::math::Vector2i GetIconSize(void);
			void SetIconSize(const mye::math::Vector2i &iconSize);

		private:

			//typedef std::pair<unsigned int, std::string> Button;

			struct Button
			{
				
				std::string icon;
				std::string text;
				std::function<void(void)> function;

				Button(void)
				{

				}

				Button(const std::string &icon,
					const CallbackType &f,
					const std::string &text) :
					icon(icon),
					text(text),
					function(f)
				{

				}

			};

			typedef std::unordered_map<unsigned int, Button> ButtonsList;

			ButtonsList m_buttons;

			HWND m_hToolbar;
			HWND m_hRebar;

			Window &m_parent;

			HIMAGELIST m_hImageList;
			mye::math::Vector2i m_iconSize;

			bool m_showText;
			bool m_vertical;

			friend class ToolbarWCR;
			static LRESULT CALLBACK WindowProc(HWND hWnd,
				UINT uMsg,
				WPARAM wParam,
				LPARAM lParam);

			

		};

	}

}
