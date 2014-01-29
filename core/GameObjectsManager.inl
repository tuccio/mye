namespace mye
{

	namespace core
	{

		size_t GameObjectHandleIDHasher::operator()(const mye::core::GameObjectHandle &hObj) const
		{
			return std::hash<int>() (hObj.id);
		}

		bool operator== (const GameObjectHandle& a,
			const GameObjectHandle& b)
		{
			return a.id == b.id &&
				a.allocation == a.allocation;
		}

		GameObject* GameObjectsManager::Get(const GameObjectHandle &hObj)
		{

			auto it = m_objects.find(hObj);

			if (it != m_objects.end() &&
				it->first.allocation == hObj.allocation)
			{
				return it->second;
			}

			return NULL;

		}

		GameObjectHandle GameObjectsManager::CreateHandle(void)
		{

			if (m_freeHandles.empty())
			{
				return GameObjectHandle(++m_lastId, 0);
			}
			else
			{
				GameObjectHandle hObj = m_freeHandles.front();
				m_freeHandles.pop_front();
				hObj.allocation++;
				return hObj;
			}

		}

		void GameObjectsManager::FreeHandle(const GameObjectHandle &hObj)
		{
			m_freeHandles.push_front(hObj);
		}

	}

}