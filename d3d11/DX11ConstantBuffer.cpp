#include "DX11ConstantBuffer.h"

#include <cassert>

using namespace mye::dx11;
using namespace mye::core;

DX11ConstantBuffer::DX11ConstantBuffer(ResourceManager *owner,
									   const std::string &name,
									   ManualResourceLoader *manual,
									   DX11Device &device) :
DX11Buffer(owner, name, manual, device),
	m_size(0)
{
}


DX11ConstantBuffer::~DX11ConstantBuffer(void)
{
}

bool DX11ConstantBuffer::LoadImpl(void)
{
	return true;
}

void DX11ConstantBuffer::UnloadImpl(void)
{
	Destroy();
}

size_t DX11ConstantBuffer::CalculateSizeImpl(void)
{
	return m_size;
}

bool DX11ConstantBuffer::Create(size_t size,
								const void *initiationData)
{

	m_size = size;

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth           = m_size;
	vertexBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	vertexBufferDesc.CPUAccessFlags      = 0;
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = initiationData;

	bool success =
		!HRTESTFAILED(m_device.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		(initiationData ? &vertexBufferData : 0x0),
		&m_buffer));

	return success;

}

void DX11ConstantBuffer::Bind(PipelineStages stage, int index)
{

	switch (stage)
	{

	case PIPELINE_VERTEX_SHADER:
		m_device.GetImmediateContext()->VSSetConstantBuffers(index, 1, &m_buffer);
		break;

	case PIPELINE_PIXEL_SHADER:
		m_device.GetImmediateContext()->PSSetConstantBuffers(index, 1, &m_buffer);
		break;

	default:
		assert(false);
		break;

	}

}

void DX11ConstantBuffer::GetData(void *data) const
{

}

void DX11ConstantBuffer::SetData(const void *data)
{
	m_device.GetImmediateContext()->UpdateSubresource(m_buffer, 0, NULL, data, 0, 0);
}