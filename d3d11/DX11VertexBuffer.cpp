#include "DX11VertexBuffer.h"

#include <mye/core/ResourceTypeManager.h>

#include <mye/d3d11/DX11Utils.h>

using namespace mye::dx11;
using namespace mye::core;

DX11VertexBuffer::DX11VertexBuffer(ResourceManager *owner,
								   const std::string &name,
								   mye::core::ManualResourceLoader *manual,
								   DX11Device &device) :
DX11Buffer(owner, name, manual, device),
	m_stride(0)
{
}


DX11VertexBuffer::~DX11VertexBuffer(void)
{
}

bool DX11VertexBuffer::LoadImpl(void)
{

	ResourceHandle hMesh = ResourceTypeManager::GetSingleton().
		CreateResource("Mesh", m_name.c_str());

	hMesh.get()->Load();

	Mesh *mesh = hMesh.Cast<Mesh>();

	return Create(mesh);

}

void DX11VertexBuffer::UnloadImpl(void)
{
	Destroy();
}

size_t DX11VertexBuffer::CalculateSizeImpl(void)
{
	return m_size;
}

bool DX11VertexBuffer::Create(Mesh *mesh)
{

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth           = mesh->GetVertexDeclaration().GetSize() *
		mesh->GetTrianglesCount() * 3;
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags      = 0;
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = mesh->GetData();

	m_stride = mesh->GetVertexDeclaration().GetSize();

	bool success =
		!HRTESTFAILED(m_device.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_buffer));
	
	return success;

}

void DX11VertexBuffer::Bind(void)
{
	UINT offset = 0;
	m_device.GetImmediateContext()->IASetVertexBuffers(0, 1, &m_buffer, &m_stride, &offset);
	m_device.GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}