#include "WindowMenu.h"

using namespace mye::win;

WindowMenu::WindowMenu(void)
{
	m_id = 0x0;
	m_parent = NULL;
}


WindowMenu::~WindowMenu(void)
{
}

WindowMenu& WindowMenu::AddSubMenu(const std::string &name)
{
	m_menu.push_back(std::pair<std::string, WindowMenu>(name, WindowMenu()));
	m_menu.back().second.m_id = IDGenerator::GetSingleton().Generate();
	m_usedIds.insert(m_menu.back().second.m_id);
	m_menu.back().second.m_parent = this;
	return m_menu.back().second;
}

WindowMenu::MenusList& WindowMenu::GetSubMenus(void)
{
	return m_menu;
}

IDGenerator::ID WindowMenu::GetID(void) const
{
	return m_id;
}

WindowMenu* WindowMenu::GetParent(void)
{
	return m_parent;
}

WindowMenu& WindowMenu::operator[] (const std::string &name)
{

	for (auto &item : m_menu)
	{
		if (item.first == name)
		{
			return item.second;
		}
	}

	assert(false);
	return *this;

}

bool WindowMenu::Contains(IDGenerator::ID id) const
{

	auto it = m_usedIds.find(id);

	if (it != m_usedIds.end())
	{
		return true;
	}

	for (auto &item : m_menu)
	{
		if (item.second.Contains(id))
		{
			return true;
		}
	}

	return false;

}