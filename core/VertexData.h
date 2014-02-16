#pragma once

#include "VertexDeclaration.h"
#include "VertexData.h"

#include <vector>

namespace mye
{

	namespace core
	{

		class VertexData
		{

		public:

			VertexData(void);
			VertexData(const VertexDeclaration &declaration);

			~VertexData(void);

			void Allocate(const VertexDeclaration &declaration, size_t n);
			void Allocate(size_t n);
			void Free(void);

			const VertexDeclaration& GetVertexDeclaration(void) const;

			void* GetData(void);
			const void* GetData(void) const;

			size_t GetVerticesCount(void) const;
			size_t GetSize(void) const;

			void SetVertexAttribute(size_t vertexIndex,
				VertexAttributeSemantic semantic,
				VertexAttributeType type,
				const void *data);

			void GetVertexAttribute(size_t vertexIndex,
				VertexAttributeSemantic semantic,
				VertexAttributeType type,
				void *data) const;

		private:

			VertexDeclaration m_declaration;
			unsigned char *m_data;
			size_t m_verticesCount;

		};

	}

}
