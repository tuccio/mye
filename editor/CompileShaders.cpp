#include "Globals.h"

#include <mye/d3d11/DX11VertexShader.h>
#include <mye/d3d11/DX11PixelShader.h>

#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

using namespace mye::core;
using namespace mye::dx11;

void CompileShaders(void)
{
	
	VertexDeclaration vDecl;

	vDecl.AddAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT3);
	vDecl.AddAttribute(VertexAttributeSemantic::TEXCOORD0, DataFormat::FLOAT2);
	vDecl.AddAttribute(VertexAttributeSemantic::NORMAL, DataFormat::FLOAT3);

	std::vector<D3D11_INPUT_ELEMENT_DESC> vDesc = MakeInputElementVector(vDecl);

	Parameters params;

	params["type"] = "vertex";
	params["inputLayoutVector"] = PointerToString(static_cast<void*>(&vDesc));

	DX11ShaderPointer hShader = ResourceTypeManager::GetSingleton().
		CreateResource<DX11Shader>(
			"DX11Shader",
			"VertexShader.hlsl",
			nullptr,
			params);

	DX11VertexShader *vertexShader = static_cast<DX11VertexShader*>(hShader.get());

	if (!hShader->Load())
	{

		const String &compileError = vertexShader->GetCompileError();

		if (!compileError.IsEmpty())
		{
			MessageBox(nullptr,
				compileError.CString(),
				"Vertex shader compile error",
				MB_OK);
		}

	}

	params.Clear();
	params["type"] = "pixel";

	hShader = ResourceTypeManager::GetSingleton().
		CreateResource<DX11Shader>(
			"DX11Shader",
			"PixelShader.hlsl",
			nullptr,
			params);

	DX11PixelShader *pixelShader = static_cast<DX11PixelShader*>(hShader.get());

	if (!hShader->Load())
	{

		MessageBox(nullptr,
			pixelShader->GetCompileError().CString(),
			"Pixel shader compile error",
			MB_OK);

	}

	vertexShader->Use();
	pixelShader->Use();

}