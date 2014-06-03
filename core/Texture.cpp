#include "Texture.h"

using namespace mye::core;

Texture::Texture(ResourceManager *owner,
				 const String &name,
				 ManualResourceLoader *manual) :
	Resource(owner, name, manual)
{
}


Texture::~Texture(void)
{
}
