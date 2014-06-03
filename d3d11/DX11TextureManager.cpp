#include "DX11TextureManager.h"

using namespace mye::dx11;

DX11TextureManager::DX11TextureManager(DX11Device &device) :
	m_device(device),
	ResourceManager("Texture")
{
}


DX11TextureManager::~DX11TextureManager(void)
{
}

DX11Texture* DX11TextureManager::CreateImpl(const mye::core::String &name,
						mye::core::ManualResourceLoader *manual,
						const mye::core::Parameters &params)
{
	DX11Texture *texture = new DX11Texture(this, name, manual, m_device);
	texture->SetParametersList(params);
	return texture;
}

void DX11TextureManager::FreeImpl(mye::core::Resource *resource)
{
	delete static_cast<DX11Texture*>(resource);
}