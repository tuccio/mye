#pragma once

#include <vector>

namespace mye
{

	namespace core
	{

		class VertexDeclaration
		{

		public:

			enum AttributeSemantic
			{
				VDA_POSITION,
				VDA_NORMAL,
				VDA_TEXCOORDS,
				VDA_AMBIENT,
				VDA_DIFFUSE,
				VDA_SPECULAR,
				VDA_COUNT
			};

			enum AttributeType
			{
				VDAT_FLOAT,
				VDAT_FLOAT2,
				VDAT_FLOAT3,
				VDAT_FLOAT4,
				VDAT_INT,
				VDAT_INT2,
				VDAT_INT3,
				VDAT_INT4,
				VDAT_COUNT
			};

			struct Attribute
			{
				AttributeSemantic semantic;
				AttributeType type;

				Attribute(AttributeSemantic semantic,
					AttributeType type)
				{
					this->semantic = semantic;
					this->type = type;
				}
			};

			static const size_t AttributeTypeSize[VDAT_COUNT];

			VertexDeclaration(void);
			~VertexDeclaration(void);

			void AddAttribute(
				AttributeSemantic semantic,
				AttributeType type);

			void InsertAttribute(
				AttributeSemantic semantic,
				AttributeType type,
				int i);

			int GetAttributeIndex(AttributeSemantic semantic) const;
			size_t GetAttributeOffset(AttributeSemantic semantic) const;

			void RemoveAttribute(int i);

			size_t GetSize(void) const;

			void Clear(void);

		private:

			std::vector<Attribute> m_attributes;
			size_t m_size;

		};

	}

}
