#include "DX11Buffer.h"
#include "DX11Utils.h"

using namespace mye::dx11;

DX11Buffer::DX11Buffer(mye::core::ResourceManager *owner,
					   const std::string &name,
					   mye::core::ManualResourceLoader *manual,
					   DX11Device &device) :
Resource(owner, name, manual),
m_device(device),
m_buffer(NULL)
{
}


DX11Buffer::~DX11Buffer(void)
{
}

ID3D11Buffer* DX11Buffer::GetBuffer(void)
{
	return m_buffer;
}

void DX11Buffer::Destroy(void)
{
	if (m_buffer)
	{
		ReleaseCOM(m_buffer);
	}
}