#include "DX11Buffer.h"
#include "DX11Utils.h"

using namespace mye::dx11;

DX11Buffer::DX11Buffer(mye::core::ResourceManager      * owner,
                       const mye::core::String         & name,
                       mye::core::ManualResourceLoader * manual) :
	GPUBuffer(owner, name, manual),
	m_buffer(nullptr)
{
}


DX11Buffer::~DX11Buffer(void)
{
}

ID3D11Buffer * DX11Buffer::GetBuffer(void)
{
	return m_buffer;
}

void DX11Buffer::Destroy(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_buffer);
}