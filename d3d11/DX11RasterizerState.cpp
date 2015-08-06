#include "DX11RasterizerState.h"

using namespace mye::dx11;
using namespace mye::core;

DX11RasterizerState::DX11RasterizerState(const RasterizerInfo & info)
{

	D3D11_RASTERIZER_DESC rasterizerDesc;

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.DepthClipEnable = TRUE;

	switch (info.cull)
	{

	case CullMode::NONE:
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		break;

	case CullMode::FRONT:
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		break;

	case CullMode::BACK:
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		break;

	}

	rasterizerDesc.FillMode = (info.wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID);

	DX11Device::GetSingleton()->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);

}


DX11RasterizerState::~DX11RasterizerState(void)
{
}

void DX11RasterizerState::Use(void)
{
	DX11Device::GetSingleton().GetImmediateContext()->RSSetState(m_rasterizerState);
}