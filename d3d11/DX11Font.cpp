#include "DX11Font.h"

#include <mye/core/ResourceTypeManager.h>

using namespace mye::dx11;
using namespace mye::core;

DX11Font::DX11Font(ResourceManager *owner,
				   const String &name,
				   ManualResourceLoader *manual) :
	Font(owner, name, manual)
{

	m_texture = ResourceTypeManager::GetSingleton().CreateResource<DX11Texture>("Texture", "./fonts/" + m_name + ".png");

}


DX11Font::~DX11Font(void)
{
}

bool DX11Font::LoadImpl(void)
{

	bool loaded = false;

	if (Font::LoadImpl() && m_texture->Load())
	{

		loaded = true;

	}

	return loaded;

}

void DX11Font::UnloadImpl(void)
{

	m_texture->Unload();

}
void DX11Font::Use(void)
{
	m_texture->Bind(0);
}