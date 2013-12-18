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

	if (_free.empty())
	{
		_objects.push_back(Allocation(new T, 0));
		return OpaqueObjectHandle<T>(_objects.size() - 1, 0);
	}
	else
	{
		
		int id = _free.front();
		_free.pop_front();

		return OpaqueObjectHandle<T>(id, _objects[id].allocation);

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
void OpaqueObjectsManager<T>::Destroy(const OpaqueObjectHandle<T> &hObj)
{

	if (hObj.id >= 0 &&
		hObj.id < _objects.size() &&
		_objects[hObj.id].allocation == hObj.allocation)
	{
		_objects[hObj.id].allocation++;
		_objects[hObj.id].object->Clear();
		_free.push_back(hObj.id);
	}

}
