#include "DX11FontManager.h"

using namespace mye::dx11;
using namespace mye::core;

DX11FontManager::DX11FontManager(void) :
	ResourceManager("Font") { }


DX11FontManager::~DX11FontManager(void) { }

DX11Font * DX11FontManager::CreateImpl(const String         & name,
                                       ManualResourceLoader * manual,
                                       const Parameters     & params)
{
	return new DX11Font(this, name, manual);
}

void DX11FontManager::FreeImpl(Resource * resource)
{
	delete static_cast<Font*>(resource);
}