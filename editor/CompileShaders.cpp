#include "Globals.h"

#include <mye/d3d11/DX11VertexShader.h>
#include <mye/d3d11/DX11PixelShader.h>

#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

using namespace mye::core;
using namespace mye::dx11;

void CompileShaders(void)
{

	Resource::ParametersList params;

	std::vector<D3D11_INPUT_ELEMENT_DESC> vDesc(1);

	vDesc[0].SemanticName         = "POSITION";
	vDesc[0].SemanticIndex        = 0;	
	vDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	vDesc[0].InputSlot            = 0;
	vDesc[0].AlignedByteOffset    = 0;
	vDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	vDesc[0].InstanceDataStepRate = 0;

	params["type"] = "vertex";
	params["inputLayoutVector"] = PointerToString(static_cast<void*>(&vDesc));

	ResourceHandle hShader = ResourceTypeManager::GetSingleton().CreateResource(
		"DX11Shader",
		"VertexShader.hlsl",
		nullptr,
		&params);

	DX11VertexShader *vertexShader = static_cast<DX11VertexShader*>(hShader.get());

	if (!hShader->Load())
	{

		const std::string &compileError = vertexShader->GetCompileError();

		if (!compileError.empty())
		{
			MessageBox(nullptr,
				compileError.c_str(),
				"Vertex shader compile error",
				MB_OK);
		}

	}

	params.clear();
	params["type"] = "pixel";

	hShader = ResourceTypeManager::GetSingleton().CreateResource(
		"DX11Shader",
		"PixelShader.hlsl",
		nullptr,
		&params);

	DX11PixelShader *pixelShader = static_cast<DX11PixelShader*>(hShader.get());

	if (!hShader->Load())
	{

		MessageBox(nullptr,
			pixelShader->GetCompileError().c_str(),
			"Pixel shader compile error",
			MB_OK);

	}

	vertexShader->Use();
	pixelShader->Use();

}