#include "DX11RasterizerState.h"

using namespace mye::dx11;
using namespace mye::core;

DX11RasterizerState::DX11RasterizerState(DX11Device &device, const RasterizerInfo &info) :
	m_device(device)
{

	D3D11_RASTERIZER_DESC rasterizerDesc;

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

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

	m_device.GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);

}


DX11RasterizerState::~DX11RasterizerState(void)
{
}

void DX11RasterizerState::Use(void)
{
	m_device.GetImmediateContext()->RSSetState(m_rasterizerState);
}