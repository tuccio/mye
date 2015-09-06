#include "Resource.h"
#include "ManualResourceLoader.h"

using namespace mye::core;

Resource::Resource(ResourceManager      * owner,
				   const String         & name,
				   ManualResourceLoader * manual) :
	INamedObject(name)
{

	m_owner         = owner;
	m_manual        = manual;
	
	m_size          = 0;

	m_paramsChanged = true;

	m_loadingState  = ResourceLoadState::NOT_LOADED;

}

Resource::Resource(void)
{

	m_owner         = nullptr;
	m_manual        = nullptr;

	m_size          = 0;

	m_paramsChanged = true;

	m_loadingState  = ResourceLoadState::NOT_LOADED;

}

Resource::~Resource(void)
{
}

bool Resource::Load(bool background)
{

	Lock();

	bool loadSuccess    = false;
	bool prepareSuccess = false;

	if (m_paramsChanged)
	{
		Unload();
	}

	if (m_loadingState == ResourceLoadState::NOT_LOADED)
	{

		if (!background)
		{

			if (!m_manual)
			{

				m_loadingState = ResourceLoadState::LOADING;
				loadSuccess = LoadImpl();

				if (loadSuccess)
				{
					m_loadingState = ResourceLoadState::LOADED;
					m_size = CalculateSizeImpl();
				}
				else
				{
					m_loadingState = ResourceLoadState::NOT_LOADED;
				}

			}
			else
			{

				m_loadingState = ResourceLoadState::LOADING;
				loadSuccess = m_manual->Load(this);

				if (loadSuccess)
				{
					m_loadingState = ResourceLoadState::LOADED;
					m_size = CalculateSizeImpl();
				}
				else
				{
					m_loadingState = ResourceLoadState::NOT_LOADED;
				}

			}

		}
		else
		{
			// TODO: Background load
		}

	}

	m_paramsChanged = false;

	Unlock();

	return m_loadingState == ResourceLoadState::LOADED;

}

void Resource::Unload(bool background)
{

	Lock();

	if (m_loadingState == ResourceLoadState::LOADED)
	{

		if (!background)
		{
			m_loadingState = ResourceLoadState::UNLOADING;
			UnloadImpl();
			m_loadingState = ResourceLoadState::NOT_LOADED;
		}
		else
		{
			// TODO: Background free
		}

	}

	m_size = 0;

	Unlock();

}

void Resource::Free(bool background)
{

	Lock();

	if (m_loadingState == ResourceLoadState::LOADED)
	{

		if (!background)
		{
			m_loadingState = ResourceLoadState::FREED;
			UnloadImpl();
		}
		else
		{
			// TODO: Background free
		}

	}

	m_size = 0;

	Unlock();

}

size_t Resource::GetSize(void) const
{
	return m_size;
}

ResourceLoadState Resource::GetState(void) const
{
	return m_loadingState;
}

bool Resource::LoadImpl(void)
{
	return true;
}

void Resource::UnloadImpl(void)
{

}

void Resource::SetParametersList(const Parameters & params)
{
	Lock();
	m_paramsChanged = true;
	m_params        = params;
	Unlock();
}

Parameters Resource::GetParametersList(void) const
{
	return m_params;
}

void Resource::CalculateSize(void)
{
	m_size = CalculateSizeImpl();
}