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

void DX11ConstantBuffer::GetData(void *data) const
{

}

void DX11ConstantBuffer::SetData(const void *data)
{
	DX11Device::GetSingleton().GetImmediateContext()->UpdateSubresource(m_buffer, 0, nullptr, data, 0, 0);
}