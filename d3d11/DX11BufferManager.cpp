#include "DX11BufferManager.h"
#include "DX11VertexBuffer.h"
#include "DX11ConstantBuffer.h"


using namespace mye::core;
using namespace mye::dx11;

DX11BufferManager::DX11BufferManager(DX11Device &device) :
	ResourceManager("DX11Buffer"),
	m_device(device)
{
}


DX11BufferManager::~DX11BufferManager(void)
{
}

ResourceHandle DX11BufferManager::CreateImpl(const mye::core::String &name,
												   ManualResourceLoader *manual,
												   Resource::ParametersList *params)
{

	if (params)
	{

		auto it = params->find("type");

		if (it != params->end())
		{

			if (it->second == "vertex")
			{
				return ResourceHandle(new DX11VertexBuffer(this, name, manual, m_device));
			}
			else if (it->second == "constant")
			{
				return ResourceHandle(new DX11ConstantBuffer(this, name, manual, m_device));
			}

		}

	}

	return ResourceHandle(nullptr);

}

void DX11BufferManager::FreeImpl(Resource* resource)
{
	static_cast<DX11Buffer*>(resource)->Destroy();
}