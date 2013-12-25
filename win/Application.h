#pragma once

#include "Window.h"

namespace mye
{

	namespace win
	{

		class Application
		{

		public:

			Application(void);
			~Application(void);

			Window& GetMainWindow(void);
			const Window& GetMainWindow(void) const;

			void Update(float dt);
			void Render(void);

			void Run(void);

		private:

			Window _mainWindow;

		};

	}

}

