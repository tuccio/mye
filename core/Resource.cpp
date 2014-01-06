#include "Resource.h"

using namespace mye::core;

Resource::Resource(ResourceManager *owner,
				   const std::string &name,
				   const std::string &group,
				   ManualResourceLoader *manual) :
	INamedObject(name)
{

	_group = group;
	_owner = owner;
	_manual = manual;
	
	_size = 0;
	_loadingState = RESOURCE_NOTLOADED;

}

Resource::Resource(void)
{

	_owner = NULL;
	_manual = NULL;

	_size = 0;
	_loadingState = RESOURCE_NOTLOADED;

}

Resource::~Resource(void)
{
}

bool Resource::Load(bool background)
{

	Lock();

	bool loadSuccess = false;
	bool prepareSuccess = false;

	if (_loadingState == RESOURCE_NOTLOADED)
	{

		if (!background)
		{

			if (!_manual)
			{

				_loadingState = RESOURCE_LOADING;
				loadSuccess = LoadImpl();

				if (loadSuccess)
				{

					_loadingState = RESOURCE_PREPARING;
					prepareSuccess = PrepareImpl();

					if (prepareSuccess)
					{
						_loadingState = RESOURCE_LOADED;
						_size = CalculateSizeImpl();
					}
					else
					{
						_loadingState = RESOURCE_NOTLOADED;
					}

				}

			}
			else
			{

				_loadingState = RESOURCE_LOADING;
				loadSuccess = _manual->Load(this);

				if (loadSuccess)
				{

					_loadingState = RESOURCE_PREPARING;
					prepareSuccess = _manual->Prepare(this);

					if (prepareSuccess)
					{
						_loadingState = RESOURCE_LOADED;
						_size = CalculateSizeImpl();
					}
					else
					{
						_loadingState = RESOURCE_NOTLOADED;
					}

				}

			}

		}
		else
		{
			// TODO
		}

	}

	Unlock();

	return loadSuccess && prepareSuccess;

}

void Resource::Unload(bool background)
{

	Lock();

	if (_loadingState == RESOURCE_LOADED)
	{

		if (!background)
		{
			_loadingState = RESOURCE_UNLOADING;
			UnloadImpl();
			_loadingState = RESOURCE_NOTLOADED;
		}
		else
		{
			// TODO
		}

	}

	_size = 0;

	Unlock();

}

void Resource::Free(bool background)
{

	Lock();

	if (_loadingState == RESOURCE_LOADED)
	{

		if (!background)
		{
			_loadingState = RESOURCE_FREED;
			UnloadImpl();
		}
		else
		{
			// TODO
		}

	}

	_size = 0;

	Unlock();

}

void Resource::SetSize(size_t size)
{
	_size = size;
}

size_t Resource::GetSize(void) const
{
	return _size;
}

Resource::LoadingState Resource::GetState(void) const
{
	return _loadingState;
}

bool Resource::LoadImpl(void)
{
	return true;
}

bool Resource::PrepareImpl(void)
{
	return true;
}

void Resource::UnloadImpl(void)
{

}

void Resource::SetParametersList(const ParametersList &params)
{
	_params = params;
}

Resource::ParametersList Resource::GetParametersList(void) const
{
	return _params;
}