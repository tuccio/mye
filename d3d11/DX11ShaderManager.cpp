#include "DX11ShaderManager.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"

using namespace mye::dx11;
using namespace mye::core;
using namespace std;

DX11ShaderManager::DX11ShaderManager(DX11Device &device) :
	ResourceManager("DX11Shader"),
	m_device(device)
{
}


DX11ShaderManager::~DX11ShaderManager(void)
{
}

ResourceHandle DX11ShaderManager::CreateImpl(const std::string &name,
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
				return ResourceHandle(new DX11VertexShader(this, name, manual, m_device));
			}
			else if (it->second == "pixel")
			{
				return ResourceHandle(new DX11PixelShader(this, name, manual, m_device));
			}

		}

	}

	return ResourceHandle(new DX11Shader(this, name));

}

void DX11ShaderManager::FreeImpl(Resource* resource)
{
	static_cast<DX11Shader*>(resource)->Destroy();
}