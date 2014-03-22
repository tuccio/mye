#include "DX11ShaderManager.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"

#include <mye/core/Utils.h>

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

DX11Shader* DX11ShaderManager::CreateImpl(const String &name,
										  ManualResourceLoader *manual,
										  const Resource::ParametersList &params)
{

	bool precompiled = false;

	auto it =  params.find("precompiled");

	if (it != params.end() && it->second == "true")
	{
		precompiled = true;
	}

	it = params.find("type");

	if (it != params.end())
	{

		if (it->second == "vertex")
		{
			return (new DX11VertexShader(this, name, manual, m_device, precompiled));
		}
		else if (it->second == "pixel")
		{
			return (new DX11PixelShader(this, name, manual, m_device, precompiled));
		}

	}

	return nullptr;

}

void DX11ShaderManager::FreeImpl(Resource* resource)
{
	static_cast<DX11Shader*>(resource)->Destroy();
}

DX11ShaderPointer DX11ShaderManager::CreateVertexShader(const String &name,
														bool precompiled,
														const std::vector<D3D11_INPUT_ELEMENT_DESC> &vdesc)
{

	Resource::ParametersList params;

	params["type"] = "vertex";
	params["precompiled"] = ToString(precompiled);
	params["inputLayoutVector"] = PointerToString(static_cast<const void*>(&vdesc));

	DX11VertexShaderPointer shader = CreateResource<DX11VertexShader>(name, nullptr, params);

	if (shader)
	{
		shader->Load();
	}

	return shader;

}

DX11ShaderPointer DX11ShaderManager::CreatePixelShader(const String &name, bool precompiled)
{

	Resource::ParametersList params;

	params["type"] = "pixel";
	params["precompiled"] = ToString(precompiled);

	DX11PixelShaderPointer shader = CreateResource<DX11PixelShader>(name, nullptr, params);

	if (shader)
	{
		shader->Load();
	}

	return shader;

}