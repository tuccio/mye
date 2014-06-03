#include "ImageManager.h"

#include <IL/il.h>

using namespace mye::core;

ImageManager::ImageManager(void) :
	ResourceManager("Image")
{
	ilInit();
}


ImageManager::~ImageManager(void)
{
	ilShutDown();
}

Image* ImageManager::CreateImpl(const String &name,
								ManualResourceLoader *manual,
								const Parameters &params)
{
	return new Image(this, name, manual);
}

void ImageManager::FreeImpl(Resource *resource)
{
	delete static_cast<Image*>(resource);
}