#include "VertexData.h"

using namespace mye::core;

VertexData::VertexData(void) :
	m_data(NULL),
	m_verticesCount(0)
{
}

VertexData::VertexData(const VertexDeclaration &declaration) :
	m_declaration(declaration),
	m_data(NULL),
	m_verticesCount(0)
{
}


VertexData::~VertexData(void)
{
	Free();
}

void VertexData::Allocate(const VertexDeclaration &declaration,
						  size_t n)
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
		m_data = NULL;
	}
}

const VertexDeclaration& VertexData::GetVertexDeclaration(void) const
{
	return m_declaration;
}

void* VertexData::GetData(void)
{
	return m_data;
}

const void* VertexData::GetData(void) const
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
									VertexDeclaration::AttributeSemantic semantic,
									VertexDeclaration::AttributeType type,
									const void *data)
{
	memcpy(
		m_data +
			vertexIndex * m_declaration.GetSize() +
			m_declaration.GetAttributeOffset(semantic),
		data,
		VertexDeclaration::AttributeTypeSize[type]);
}

void VertexData::GetVertexAttribute(size_t vertexIndex,
									VertexDeclaration::AttributeSemantic semantic,
									VertexDeclaration::AttributeType type,
									void *data) const
{
	memcpy(
		data,
		m_data +
			vertexIndex * m_declaration.GetSize() +
			m_declaration.GetAttributeOffset(semantic),
		VertexDeclaration::AttributeTypeSize[type]);
}