#include "DX11ConstantBuffer.h"

#include <cassert>

using namespace mye::dx11;
using namespace mye::core;

DX11ConstantBuffer::DX11ConstantBuffer(void) :
	DX11Buffer(nullptr, "", nullptr),
	m_size(0) { }

DX11ConstantBuffer::DX11ConstantBuffer(ResourceManager      * owner,
									   const String         & name,
									   ManualResourceLoader * manual) :
	DX11Buffer(owner, name, manual),
	m_size(0) { }


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

bool DX11ConstantBuffer::Create(size_t size, const void * initiationData)
{

	m_size = size;

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth           = m_size;
	vertexBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	vertexBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = initiationData;

	bool success =
		!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->
			CreateBuffer(&vertexBufferDesc,
		                 (initiationData ? &vertexBufferData : nullptr),
		                 &m_buffer));

	return success;

}

void DX11ConstantBuffer::Bind(DX11PipelineStage stage, int index)
{

	switch (stage)
	{

	case DX11PipelineStage::VERTEX_SHADER:
		DX11Device::GetSingleton().GetImmediateContext()->VSSetConstantBuffers(index, 1, &m_buffer);
		break;

	case DX11PipelineStage::PIXEL_SHADER:
		DX11Device::GetSingleton().GetImmediateContext()->PSSetConstantBuffers(index, 1, &m_buffer);
		break;

	case DX11PipelineStage::GEOMETRY_SHADER:
		DX11Device::GetSingleton().GetImmediateContext()->GSSetConstantBuffers(index, 1, &m_buffer);
		break;

	case DX11PipelineStage::COMPUTE_SHADER:
		DX11Device::GetSingleton().GetImmediateContext()->CSSetConstantBuffers(index, 1, &m_buffer);
		break;

	default:
		assert(false && "Unsupported DirectX stage");
		break;

	}

}

bool DX11ConstantBuffer::GetData(void * data) const
{

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;

	if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton().GetImmediateContext()->Map(m_buffer, 0, D3D11_MAP_READ, 0, &mappedBuffer)))
	{
		memcpy(data, mappedBuffer.pData, m_size);
		DX11Device::GetSingleton().GetImmediateContext()->Unmap(m_buffer, 0);
		return true;
	}

	return false;

}

bool DX11ConstantBuffer::SetData(const void * data)
{

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;

	if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton().GetImmediateContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer)))
	{
		memcpy(mappedBuffer.pData, data, m_size);
		DX11Device::GetSingleton().GetImmediateContext()->Unmap(m_buffer, 0);
		return true;
	}

	return false;

}

bool DX11ConstantBuffer::SetSubData(const void * data, size_t offset, size_t size)
{

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;

	if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton().GetImmediateContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer)))
	{
		memcpy(mappedBuffer.pData, (const uint8_t *) data + offset, size);
		DX11Device::GetSingleton().GetImmediateContext()->Unmap(m_buffer, 0);
		return true;
	}

	return false;

}