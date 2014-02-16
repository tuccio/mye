#pragma once

#include <vector>

namespace mye
{

	namespace core
	{

		enum class VertexAttributeSemantic
		{
			POSITION,
			NORMAL,
			TEXCOORDS,
			AMBIENT,
			DIFFUSE,
			SPECULAR,
			COUNT
		};

		enum class VertexAttributeType
		{
			FLOAT,
			FLOAT2,
			FLOAT3,
			FLOAT4,
			INT,
			INT2,
			INT3,
			INT4,
			COUNT
		};


		class VertexDeclaration
		{

		public:

			struct Attribute
			{
				VertexAttributeSemantic semantic;
				VertexAttributeType type;

				Attribute(VertexAttributeSemantic semantic,
					VertexAttributeType type)
				{
					this->semantic = semantic;
					this->type = type;
				}
			};

			static const size_t AttributeTypeSize[VertexAttributeType::COUNT];

			VertexDeclaration(void);
			~VertexDeclaration(void);

			void AddAttribute(
				VertexAttributeSemantic semantic,
				VertexAttributeType type);

			void InsertAttribute(
				VertexAttributeSemantic semantic,
				VertexAttributeType type,
				int i);

			int GetAttributeIndex(VertexAttributeSemantic semantic) const;
			size_t GetAttributeOffset(VertexAttributeSemantic semantic) const;

			void RemoveAttribute(int i);

			size_t GetSize(void) const;

			void Clear(void);

		private:

			std::vector<Attribute> m_attributes;
			size_t m_size;

		};

	}

}
