#include "DX11BufferManager.h"
#include "DX11VertexBuffer.h"
#include "DX11ConstantBuffer.h"


using namespace mye::core;
using namespace mye::dx11;

DX11BufferManager::DX11BufferManager(void) :
	ResourceManager("GPUBuffer")
{
}


DX11BufferManager::~DX11BufferManager(void)
{
}

Resource * DX11BufferManager::CreateImpl(const mye::core::String & name,
										  ManualResourceLoader * manual,
										  const Parameters & params)
{

	if (params.Contains("type"))
	{

		String type = params.GetString("type");

		if (type == "vertex")
		{
			return static_cast<Resource*>(new DX11VertexBuffer(this, name, manual));
		}
		else if (type == "constant")
		{
			return static_cast<Resource*>(new DX11ConstantBuffer(this, name, manual));
		}

	}

	return (nullptr);

}

void DX11BufferManager::FreeImpl(Resource * resource)
{
	static_cast<DX11Buffer*>(resource)->Destroy();
}