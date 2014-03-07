#include "ResourceHandle.h"

using namespace mye::core;

ResourceHandle::ResourceHandle(void) :
	boost::shared_ptr<Resource>()
{
}

ResourceHandle::ResourceHandle(Resource *resource) :
	boost::shared_ptr<Resource>(resource)
{
}

ResourceHandle::ResourceHandle(Resource &resource) :
	boost::shared_ptr<Resource>(&resource)
{
}

ResourceHandle::~ResourceHandle(void)
{
}

/*

ResourceHandle::operator bool (void) const
{
	return _ptr != nullptr;
}

Resource& ResourceHandle::operator* (void)
{
	return *_ptr;
}

const Resource& ResourceHandle::operator* (void) const
{
	return *_ptr;
}

Resource* ResourceHandle::operator-> (void)
{
	return &*_ptr;
}

const Resource* ResourceHandle::operator-> (void) const
{
	return &*_ptr;
}*/