template <typename T>
OpaqueObjectHandle<T>::OpaqueObjectHandle(void) :
	id(-1),
	allocation(-1)
{
}

template <typename T>
OpaqueObjectHandle<T>::OpaqueObjectHandle(int id, int allocation) :
	id(id),
	allocation(allocation)
{
}

template <typename T>
OpaqueObjectsManager<T>::Allocation::Allocation(void) :
	object(NULL),
	allocation(-1)
{
}

template <typename T>
OpaqueObjectsManager<T>::Allocation::Allocation(T *object, int allocation) :
	object(object),
	allocation(allocation)
{
}

template <typename T>
OpaqueObjectsManager<T>::OpaqueObjectsManager(void)
{
}

template <typename T>
OpaqueObjectsManager<T>::~OpaqueObjectsManager(void)
{
}

template <typename T>
OpaqueObjectHandle<T> OpaqueObjectsManager<T>::Create(void)
{
	return Create(std::string());
}

template <typename T>
OpaqueObjectHandle<T> OpaqueObjectsManager<T>::Create(const std::string &name)
{

	OpaqueObjectHandle<T> handle;

	if (m_freeIds.empty())
	{
		m_objects.push_back(Allocation(new T, 0));
		handle = OpaqueObjectHandle<T>(m_objects.size() - 1, 0);
	}
	else
	{

		int id = m_freeIds.front();
		m_freeIds.pop_front();

		handle = OpaqueObjectHandle<T>(m_objects.size() - 1, 0);

	}

	T *object = m_objects[handle.id].object;

	object->SetName(name);
	object->OnCreation(this, handle);	

	if (!name.empty())
	{		
		m_names.insert(std::pair<std::string, OpaqueObjectHandle<T>>(name, handle));
	}	

	return handle;

}

template <typename T>
void OpaqueObjectsManager<T>::Destroy(const OpaqueObjectHandle<T> &hObj)
{

	if (hObj.id >= 0 &&
		hObj.id < m_objects.size() &&
		m_objects[hObj.id].allocation == hObj.allocation)
	{
		
		m_objects[hObj.id].allocation++;
		m_objects[hObj.id].object->OnDestruction();
		m_freeIds.push_back(hObj.id);

		std::string name = m_objects[hObj.id].object->GetName();

		if (!name.empty())
		{	

			for (auto eqr = m_names.equal_range(name);
				 eqr.first != eqr.second;
				 eqr.first++)
			{

				if (eqr.first->second == hObj)
				{
					m_names.erase(eqr.first);
					break;
				}

			}

		}

	}

}

template <typename T>
T* OpaqueObjectsManager<T>::Get(const OpaqueObjectHandle<T> &hObj)
{

	if (hObj.id >= 0 &&
		hObj.id < m_objects.size() &&
		m_objects[hObj.id].allocation == hObj.allocation)
	{
		return m_objects[hObj.id].object;
	}

	return NULL;

}

template <typename T>
OpaqueObjectHandle<T> OpaqueObjectsManager<T>::Find(const std::string &name)
{

	auto eqr = m_names.equal_range(name);

	if (eqr.first == m_names.end())
	{
		return OpaqueObjectHandle<T>(-1, -1);
	}
	
	return eqr.first->second;

}