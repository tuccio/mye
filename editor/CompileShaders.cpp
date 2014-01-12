#include "Globals.h"

#include <mye/d3d11/DX11VertexShader.h>
#include <mye/d3d11/DX11PixelShader.h>

using namespace mye::core;
using namespace mye::dx11;

void CompileShaders(void)
{

	Resource::ParametersList params;

	params["type"] = "vertex";

	ResourceHandle hShader = g_shaderManager.CreateResource(
		"VertexShader.hlsl",
		NULL,
		&params);

	DX11VertexShader *vertexShader = static_cast<DX11VertexShader*>(hShader.get());

	if (!hShader->Load())
	{

		MessageBox(NULL,
			vertexShader->GetCompileError().c_str(),
			"Vertex shader compile error",
			MB_OK);

	}

	params["type"] = "pixel";

	hShader = g_shaderManager.CreateResource(
		"PixelShader.hlsl",
		NULL,
		&params);

	DX11PixelShader *pixelShader = static_cast<DX11PixelShader*>(hShader.get());

	if (!hShader->Load())
	{

		MessageBox(NULL,
			pixelShader->GetCompileError().c_str(),
			"Pixel shader compile error",
			MB_OK);

	}

	g_device.GetImmediateContext()->VSSetShader(
		vertexShader->GetVertexShader(),
		NULL,
		0);

	g_device.GetImmediateContext()->PSSetShader(
		pixelShader->GetPixelShader(),
		NULL,
		0);

}