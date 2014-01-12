#include "Resource.h"

using namespace mye::core;

Resource::Resource(ResourceManager *owner,
				   const std::string &name,
				   ManualResourceLoader *manual) :
	INamedObject(name)
{

	m_owner = owner;
	m_manual = manual;
	
	m_size = 0;
	m_loadingState = RESOURCE_NOTLOADED;

}

Resource::Resource(void)
{

	m_owner = NULL;
	m_manual = NULL;

	m_size = 0;
	m_loadingState = RESOURCE_NOTLOADED;

}

Resource::~Resource(void)
{
}

bool Resource::Load(bool background)
{

	Lock();

	bool loadSuccess = false;
	bool prepareSuccess = false;

	if (m_loadingState == RESOURCE_NOTLOADED)
	{

		if (!background)
		{

			if (!m_manual)
			{

				m_loadingState = RESOURCE_LOADING;
				loadSuccess = LoadImpl();

				if (loadSuccess)
				{

					m_loadingState = RESOURCE_PREPARING;
					prepareSuccess = PrepareImpl();

					if (prepareSuccess)
					{
						m_loadingState = RESOURCE_LOADED;
						m_size = CalculateSizeImpl();
					}
					else
					{
						m_loadingState = RESOURCE_NOTLOADED;
					}

				}

			}
			else
			{

				m_loadingState = RESOURCE_LOADING;
				loadSuccess = m_manual->Load(this);

				if (loadSuccess)
				{

					m_loadingState = RESOURCE_PREPARING;
					prepareSuccess = m_manual->Prepare(this);

					if (prepareSuccess)
					{
						m_loadingState = RESOURCE_LOADED;
						m_size = CalculateSizeImpl();
					}
					else
					{
						m_loadingState = RESOURCE_NOTLOADED;
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

	if (m_loadingState == RESOURCE_LOADED)
	{

		if (!background)
		{
			m_loadingState = RESOURCE_UNLOADING;
			UnloadImpl();
			m_loadingState = RESOURCE_NOTLOADED;
		}
		else
		{
			// TODO
		}

	}

	m_size = 0;

	Unlock();

}

void Resource::Free(bool background)
{

	Lock();

	if (m_loadingState == RESOURCE_LOADED)
	{

		if (!background)
		{
			m_loadingState = RESOURCE_FREED;
			UnloadImpl();
		}
		else
		{
			// TODO
		}

	}

	m_size = 0;

	Unlock();

}

size_t Resource::GetSize(void) const
{
	return m_size;
}

Resource::LoadingState Resource::GetState(void) const
{
	return m_loadingState;
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
	m_params = params;
}

Resource::ParametersList Resource::GetParametersList(void) const
{
	return m_params;
}