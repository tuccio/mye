#include "Lockable.h"

using namespace mye::core;

Lockable::Lockable(void)
{
}


Lockable::~Lockable(void)
{
}

void Lockable::Lock(void)
{
	MYE_RECURSIVE_MUTEX_LOCK(mMutex);
}

void Lockable::Unlock(void)
{
	MYE_RECURSIVE_MUTEX_UNLOCK(mMutex);
}