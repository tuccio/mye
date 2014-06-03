#include "FontManager.h"

#include "Font.h"

using namespace mye::core;

FontManager::FontManager(void) :
	ResourceManager("Font")
{
}


FontManager::~FontManager(void)
{
}

Font* FontManager::CreateImpl(const String &name,
							  ManualResourceLoader *manual,
							  const Parameters &params)
{
	return new Font(this, name, manual);
}

void FontManager::FreeImpl(Resource *resource)
{
	delete static_cast<Font*>(resource);
}