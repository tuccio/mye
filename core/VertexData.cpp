#include "VertexData.h"

using namespace mye::core;

VertexData::VertexData(void) :
	m_data(nullptr),
	m_verticesCount(0)
{
}

VertexData::VertexData(const VertexDeclaration & declaration) :
	m_declaration(declaration),
	m_data(nullptr),
	m_verticesCount(0)
{
}


VertexData::~VertexData(void)
{
	Free();
}

void VertexData::Allocate(const VertexDeclaration & declaration, size_t n)
{
	m_declaration = declaration;
	Allocate(n);
}

void VertexData::Allocate(size_t n)
{
	Free();
	m_verticesCount = n;
	m_data = new unsigned char [m_declaration.GetSize() * n];
}

void VertexData::Free(void)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
}

const VertexDeclaration & VertexData::GetVertexDeclaration(void) const
{
	return m_declaration;
}

void * VertexData::GetData(void)
{
	return m_data;
}

const void * VertexData::GetData(void) const
{
	return m_data;
}

size_t VertexData::GetVerticesCount(void) const
{
	return m_verticesCount;
}

size_t VertexData::GetSize(void) const
{
	return m_verticesCount * m_declaration.GetSize();
}

void VertexData::SetVertexAttribute(size_t vertexIndex,
									VertexAttributeSemantic semantic,
									DataFormat type,
									const void * data)
{

	size_t stride = m_declaration.GetSize();
	size_t offset = m_declaration.GetAttributeOffset(semantic);
	size_t size   = GetDataTypeSize(type);

	memcpy(m_data +	vertexIndex * stride + offset, data, size);

}

void VertexData::GetVertexAttribute(size_t vertexIndex,
									VertexAttributeSemantic semantic,
									DataFormat type,
									void * data) const
{

	size_t stride = m_declaration.GetSize();
	size_t offset = m_declaration.GetAttributeOffset(semantic);
	size_t size   = GetDataTypeSize(type);

	memcpy(data, m_data + vertexIndex * stride + offset, size);

}