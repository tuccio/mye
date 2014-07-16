#include "Module.h"
#include "Game.h"

using namespace mye::core;


Module::Module(void) :
	m_initialized(false)
{
}


Module::~Module(void)
{

	if (m_initialized)
	{
		Shutdown();
	}

}

bool Module::Init(void)
{
	
	m_initialized = true;

	return m_initialized;

}

void Module::Shutdown(void)
{

	m_initialized = false;

}

bool Module::IsInitialized(void) const
{
	return m_initialized;
}