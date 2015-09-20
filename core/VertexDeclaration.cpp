#include "VertexDeclaration.h"

#include <algorithm>

#include <boost/algorithm/string.hpp>

using namespace mye::core;

static const size_t __dataTypeSize[static_cast<unsigned int>(DataFormat::COUNT)] =
{

	// FLOAT .. FLOAT4

	sizeof(float),
	2 * sizeof(float),
	3 * sizeof(float),
	4 * sizeof(float),

	// HALF ... HALF4

	sizeof(float) / 2,
	sizeof(float),
	3 * sizeof(float) / 2,
	2 * sizeof(float),

	// INT ... INT4

	sizeof(int),
	2 * sizeof(int),
	3 * sizeof(int),
	4 * sizeof(int),

	// BYTE ... BYTE4

	sizeof(char),
	2 * sizeof(char),
	3 * sizeof(char),
	4 * sizeof(char),

	// FIXED SIZE TYPES

	24,  // RGB24
	32,  // RGBA32
	96,  // RGB96
	128, // RGBA128
	24,  // SRGB24
	32   // SRGBA32

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

bool mye::core::ParseVertexAttributeSemantic(const char * s, VertexAttributeSemantic & attribute)
{

	if (boost::iequals("position", s))
	{
		attribute = VertexAttributeSemantic::POSITION;
	}
	else if (boost::iequals("normal", s))
	{
		attribute = VertexAttributeSemantic::NORMAL;
	}
	else if (boost::iequals("texcoord0", s))
	{
		attribute = VertexAttributeSemantic::TEXCOORD0;
	}
	else if (boost::iequals("texcoord1", s))
	{
		attribute = VertexAttributeSemantic::TEXCOORD1;
	}
	else if (boost::iequals("texcoord2", s))
	{
		attribute = VertexAttributeSemantic::TEXCOORD2;
	}
	else if (boost::iequals("texcoord3", s))
	{
		attribute = VertexAttributeSemantic::TEXCOORD3;
	}
	else if (boost::iequals("texcoord4", s))
	{
		attribute = VertexAttributeSemantic::TEXCOORD4;
	}
	else if (boost::iequals("texcoord5", s))
	{
		attribute = VertexAttributeSemantic::TEXCOORD5;
	}
	else if (boost::iequals("texcoord6", s))
	{
		attribute = VertexAttributeSemantic::TEXCOORD6;
	}
	else if (boost::iequals("texcoord7", s))
	{
		attribute = VertexAttributeSemantic::TEXCOORD7;
	}
	else if (boost::iequals("tangent", s))
	{
		attribute = VertexAttributeSemantic::TANGENT;
	}
	else if (boost::iequals("bitangent", s))
	{
		attribute = VertexAttributeSemantic::BITANGENT;
	}
	else if (boost::iequals("diffuse", s))
	{
		attribute = VertexAttributeSemantic::DIFFUSE;
	}
	else if (boost::iequals("specular", s))
	{
		attribute = VertexAttributeSemantic::SPECULAR;
	}
	else
	{
		return false;
	}

	return true;

}

bool mye::core::ParseDataFormat(const char * s, DataFormat & format)
{

	if (boost::iequals("float", s))
	{
		format = DataFormat::FLOAT;
	}
	else if (boost::iequals("float2", s))
	{
		format = DataFormat::FLOAT2;
	}
	else if (boost::iequals("float3", s))
	{
		format = DataFormat::FLOAT3;
	}
	else if (boost::iequals("float4", s))
	{
		format = DataFormat::FLOAT4;
	}
	else if (boost::iequals("int", s))
	{
		format = DataFormat::INT;
	}
	else if (boost::iequals("int2", s))
	{
		format = DataFormat::INT2;
	}
	else if (boost::iequals("int3", s))
	{
		format = DataFormat::INT3;
	}
	else if (boost::iequals("int4", s))
	{
		format = DataFormat::INT4;
	}
	else
	{
		return false;
	}

	return true;

}