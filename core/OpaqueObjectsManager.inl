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

	if (_free.empty())
	{
		_objects.push_back(Allocation(new T, 0));
		handle = OpaqueObjectHandle<T>(_objects.size() - 1, 0);
	}
	else
	{

		int id = _free.front();
		_free.pop_front();

		handle = OpaqueObjectHandle<T>(_objects.size() - 1, 0);

	}

	_objects[handle.id].object->SetName(name);

	if (!name.empty())
	{		
		_names.insert(std::pair<std::string, OpaqueObjectHandle<T>>(name, handle));
	}	

	return handle;

}

template <typename T>
void OpaqueObjectsManager<T>::Destroy(const OpaqueObjectHandle<T> &hObj)
{

	if (hObj.id >= 0 &&
		hObj.id < _objects.size() &&
		_objects[hObj.id].allocation == hObj.allocation)
	{
		
		_objects[hObj.id].allocation++;
		_objects[hObj.id].object->Clear();
		_free.push_back(hObj.id);

		std::string name = _objects[hObj.id].object->GetName();

		if (!name.empty())
		{

			

			for (auto eqr = _names.equal_range(name);
				 eqr.first != eqr.second;
				 eqr.first++)
			{

				if (eqr.first->second == hObj)
				{
					_names.erase(eqr.first);
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
		hObj.id < _objects.size() &&
		_objects[hObj.id].allocation == hObj.allocation)
	{
		return _objects[hObj.id].object;
	}

	return NULL;

}

template <typename T>
OpaqueObjectHandle<T> OpaqueObjectsManager<T>::Find(const std::string &name)
{

	auto eqr = _names.equal_range(name);

	if (eqr.first == _names.end())
	{
		return OpaqueObjectHandle<T>(-1, -1);
	}
	
	return eqr.first->second;

}