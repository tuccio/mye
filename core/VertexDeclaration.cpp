#include "VertexDeclaration.h"

#include <algorithm>

using namespace mye::core;

const size_t VertexDeclaration::AttributeTypeSize[VertexAttributeType::COUNT] =
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

VertexDeclaration::VertexDeclaration(void) :
	m_size(0)
{
}


VertexDeclaration::~VertexDeclaration(void)
{
}

void VertexDeclaration::AddAttribute(
	VertexAttributeSemantic semantic,
	VertexAttributeType type)
{
	m_attributes.push_back(Attribute(semantic, type));
	m_size = AttributeTypeSize[static_cast<int>(type)];
}

void VertexDeclaration::InsertAttribute(
	VertexAttributeSemantic semantic,
	VertexAttributeType type,
	int i)
{
	m_attributes.insert(m_attributes.begin() + i,
		1,
		Attribute(semantic, type));
	m_size += AttributeTypeSize[static_cast<int>(type)];
}

int VertexDeclaration::GetAttributeIndex(VertexAttributeSemantic semantic) const
{
	
	auto it = std::find_if(m_attributes.begin(),
		m_attributes.end(),
		[semantic] (const Attribute& a)->bool { return (a.semantic) == semantic; });

	return (it == m_attributes.end() ? -1 : it - m_attributes.begin());

}

size_t VertexDeclaration::GetAttributeOffset(VertexAttributeSemantic semantic) const
{

	size_t offset = 0;
	int index = GetAttributeIndex(semantic);

	for (int i = 0; i < index; i++)
	{
		offset += AttributeTypeSize[static_cast<int>(m_attributes[i].type)];
	}

	return offset;

}

void VertexDeclaration::RemoveAttribute(int i)
{
	auto it = m_attributes.begin() + i;
	m_size -= AttributeTypeSize[static_cast<int>(it->type)];
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