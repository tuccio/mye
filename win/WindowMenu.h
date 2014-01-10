#pragma once

#include <string>
#include <set>
#include <vector>

#include "IDGenerator.h"

namespace mye
{

	namespace win
	{

		class WindowMenu
		{

		public:

			class Listener;

			typedef std::vector<std::pair<std::string, WindowMenu>> MenusList;

			WindowMenu(void);
			~WindowMenu(void);

			WindowMenu& AddSubMenu(const std::string &name);
			MenusList& GetSubMenus(void);

			IDGenerator::ID GetID(void) const;
			WindowMenu* GetParent(void);

			WindowMenu& operator[] (const std::string &name);

			bool Contains(IDGenerator::ID id) const;
			
		private:

			MenusList m_menu;
			IDGenerator::ID m_id;
			std::set<IDGenerator::ID> m_usedIds;

			WindowMenu *m_parent;

		};

		class WindowMenu::Listener
		{

		public:

			virtual inline void OnMenuSelected(IDGenerator::ID id) { }

		};

	}

}
