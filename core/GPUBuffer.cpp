#include "GPUBuffer.h"

using namespace mye::core;

GPUBuffer::GPUBuffer(ResourceManager *owner,
					 const String &name,
					 ManualResourceLoader *manual) :
Resource(owner, name, manual)
{
}


GPUBuffer::~GPUBuffer(void)
{
}
