#include "DX11VertexBuffer.h"

#include <mye/core/ResourceTypeManager.h>

#include <mye/d3d11/DX11Utils.h>

using namespace mye::dx11;
using namespace mye::core;

DX11VertexBuffer::DX11VertexBuffer(ResourceManager *owner,
								   const mye::core::String &name,
								   mye::core::ManualResourceLoader *manual,
								   DX11Device &device) :
DX11Buffer(owner, name, manual, device),
	m_stride(0),
	m_vertices(0)
{
}


DX11VertexBuffer::~DX11VertexBuffer(void)
{
}

bool DX11VertexBuffer::LoadImpl(void)
{

	ResourceHandle hMesh = ResourceTypeManager::GetSingleton().
		CreateResource("Mesh", m_name.CString());

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

bool DX11VertexBuffer::Create(void *data, size_t n, const VertexDeclaration &vDecl)
{

	if (m_buffer)
	{
		Destroy();
	}

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth           = vDecl.GetSize() * n;
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags      = 0;
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = data;

	m_stride = vDecl.GetSize();
	m_vertices = n;

	bool success =
		!HRTESTFAILED(m_device.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_buffer));

	return success;

}

bool DX11VertexBuffer::Create(Mesh *mesh)
{

	if (m_buffer)
	{
		Destroy();
	}

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	/*vertexBufferDesc.ByteWidth           = mesh->GetVertexDeclaration().GetSize() *
		mesh->GetTrianglesCount() * 3;*/
	vertexBufferDesc.ByteWidth           = mesh->GetSize();
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags      = 0;
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = mesh->GetData();

	m_stride = mesh->GetVertexDeclaration().GetSize();
	m_vertices = mesh->GetTrianglesCount() * 3;

	bool success =
		!HRTESTFAILED(m_device.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_buffer));
	
	return success;

}

bool DX11VertexBuffer::Create(Model *model)
{

	if (m_buffer)
	{
		Destroy();
	}

	int meshesCount = model->GetMeshesCount();
	size_t offset = 0;
	std::vector<unsigned char> data(model->GetSize());

	m_vertices = 0;

	for (int i = 0; i < meshesCount; i++)
	{

		Mesh* mesh = model->GetMesh(i);
		size_t size = mesh->GetSize();

		memcpy(&data[0] + offset,
			mesh->GetData(),
			size);

		offset += size;
		m_vertices += mesh->GetTrianglesCount() * 3;

	}

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth           = model->GetSize();
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags      = 0;
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData, *pVertexBufferData;

	if (data.size() > 0)
	{
		pVertexBufferData = &vertexBufferData;
		vertexBufferData.pSysMem = &data[0];
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
	}
	else
	{
		pVertexBufferData = nullptr;
	}
	

	bool success =
		!HRTESTFAILED(m_device.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		pVertexBufferData,
		&m_buffer));

	

	if (success && meshesCount > 0)
	{
		m_stride = model->GetMesh(0)->GetVertexDeclaration().GetSize();
	}
	else
	{
		m_stride = 0;
	}

	return success;

}

void DX11VertexBuffer::Bind(void)
{
	UINT offset = 0;
	m_device.GetImmediateContext()->IASetVertexBuffers(0, 1, &m_buffer, &m_stride, &offset);
}

size_t DX11VertexBuffer::GetVerticesCount(void) const
{
	return m_vertices;
}