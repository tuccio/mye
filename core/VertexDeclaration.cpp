#include "VertexDeclaration.h"

#include <algorithm>

using namespace mye::core;

static const size_t __dataTypeSize[static_cast<unsigned int>(DataFormat::COUNT)] =
{

	sizeof(float),
	2 * sizeof(float),
	3 * sizeof(float),
	4 * sizeof(float),
	sizeof(int),
	2 * sizeof(int),
	3 * sizeof(int),
	4 * sizeof(int)

};

size_t mye::core::GetDataTypeSize(DataFormat type)
{
	return __dataTypeSize[static_cast<unsigned int>(type)];
}

VertexDeclaration::VertexDeclaration(void) :
	m_size(0)
{
}

VertexDeclaration::VertexDeclaration(std::initializer_list<VertexAttribute> initializerList) :
	m_size(0)
{

	for (const VertexAttribute &a : initializerList)
	{
		AddAttribute(a.semantic, a.type);
	}

}


VertexDeclaration::~VertexDeclaration(void)
{
}

void VertexDeclaration::AddAttribute(
	VertexAttributeSemantic semantic,
	DataFormat type)
{
	m_attributes.push_back(VertexAttribute(semantic, type));
	m_size += __dataTypeSize[static_cast<unsigned int>(type)];
}

void VertexDeclaration::InsertAttribute(
	VertexAttributeSemantic semantic,
	DataFormat type,
	int i)
{
	m_attributes.insert(m_attributes.begin() + i,
		1,
		VertexAttribute(semantic, type));
	m_size += __dataTypeSize[static_cast<unsigned int>(type)];
}

int VertexDeclaration::GetAttributeIndex(VertexAttributeSemantic semantic) const
{
	
	auto it = std::find_if(m_attributes.begin(),
		m_attributes.end(),
		[semantic](const VertexAttribute& a)->bool { return (a.semantic) == semantic; });

	return (it == m_attributes.end() ? -1 : it - m_attributes.begin());

}

size_t VertexDeclaration::GetAttributeOffset(VertexAttributeSemantic semantic) const
{

	size_t offset = 0;
	int index = GetAttributeIndex(semantic);

	for (int i = 0; i < index; i++)
	{
		offset += __dataTypeSize[static_cast<unsigned int>(m_attributes[i].type)];
	}

	return offset;

}

void VertexDeclaration::RemoveAttribute(int i)
{
	auto it = m_attributes.begin() + i;
	m_size -= __dataTypeSize[static_cast<unsigned int>(it->type)];
	m_attributes.erase(it);
}

void VertexDeclaration::Clear(void)
{
	m_attributes.clear();
	m_size = 0;
}

size_t VertexDeclaration::GetSize(void) const
{
	return m_size;
}

VertexDeclaration::Iterator VertexDeclaration::begin() const
{
	return m_attributes.begin();
}

VertexDeclaration::Iterator VertexDeclaration::end() const
{
	return m_attributes.end();
}