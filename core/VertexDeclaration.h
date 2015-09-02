#pragma once

#include <vector>
#include <initializer_list>

namespace mye
{

	namespace core
	{

		enum class VertexAttributeSemantic
		{
			POSITION,
			NORMAL,
			TEXCOORD0,
			TEXCOORD1,
			TEXCOORD2,
			TEXCOORD3,
			TEXCOORD4,
			TEXCOORD5,
			TEXCOORD6,
			TEXCOORD7,
			TANGENT,
			BITANGENT,
			AMBIENT,
			DIFFUSE,
			SPECULAR,
			COUNT
		};

		enum class DataFormat
		{
			FLOAT,
			FLOAT2,
			FLOAT3,
			FLOAT4,
			HALF,
			HALF2,
			HALF3,
			HALF4,
			INT,
			INT2,
			INT3,
			INT4,
			BYTE,
			BYTE2,
			BYTE3,
			BYTE4,
			RGB24,
			RGBA32,
			RGB96,
			RGBA128,
			sRGB24,
			sRGBA32,
			COUNT
		};

		size_t GetDataTypeSize(DataFormat type);

		bool ParseVertexAttributeSemantic(const char * s, VertexAttributeSemantic & attribute);
		bool ParseDataFormat(const char * s, DataFormat & format);

		struct VertexAttribute
		{

			VertexAttributeSemantic semantic;
			DataFormat type;

			VertexAttribute(VertexAttributeSemantic semantic,
				DataFormat type) :
				semantic(semantic),
				type(type)
			{
			}

		};

		class VertexDeclaration
		{

		public:

			typedef std::vector<VertexAttribute>::const_iterator Iterator;

			VertexDeclaration(void);
			VertexDeclaration(std::initializer_list<VertexAttribute> initializerList);

			~VertexDeclaration(void);

			void AddAttribute(
				VertexAttributeSemantic semantic,
				DataFormat type);

			void InsertAttribute(
				VertexAttributeSemantic semantic,
				DataFormat type,
				int i);

			int GetAttributeIndex(VertexAttributeSemantic semantic) const;
			size_t GetAttributeOffset(VertexAttributeSemantic semantic) const;

			void RemoveAttribute(int i);

			size_t GetSize(void) const;

			void Clear(void);

			Iterator begin() const;
			Iterator end() const;

		private:

			std::vector<VertexAttribute> m_attributes;
			size_t m_size;

		};

	}

}
