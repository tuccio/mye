#pragma once

#include "VertexData.h"
#include "Resource.h"

namespace mye
{

	namespace core
	{

		class Mesh :
			public Resource
		{

		public:

			Mesh(ResourceManager *resourceManager,
				const std::string &name,
				ManualResourceLoader *loader);

			~Mesh(void);

			void Allocate(VertexDeclaration declaration,
				size_t triangles);

			void Allocate(size_t triangles);

			void Destroy(void);

			size_t GetTrianglesCount(void) const;

			const VertexDeclaration& GetVertexDeclaration(void) const;

			const void* GetData(void) const;

			void SetVertexAttribute(size_t triangleIndex,
				size_t vertexIndex,
				VertexDeclaration::AttributeSemantic semantic,
				VertexDeclaration::AttributeType type,
				const void *data);

			void GetVertexAttribute(size_t triangleIndex,
				size_t vertexIndex,
				VertexDeclaration::AttributeSemantic semantic,
				VertexDeclaration::AttributeType type,
				void *data) const;

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			VertexData m_data;
			size_t m_triangles;

		};

	}

}
