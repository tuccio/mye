#pragma once

#include "VertexData.h"
#include "Resource.h"

#include <mye/math/Math.h>

#include <utility>

namespace mye
{

	namespace core
	{

		class Mesh :
			public Resource
		{

		public:

			typedef std::pair<mye::math::Vector3, mye::math::Vector3> VectorPair;

			Mesh(ResourceManager *resourceManager,
				const String &name,
				ManualResourceLoader *loader);

			~Mesh(void);

			void Allocate(VertexDeclaration declaration,
				size_t triangles);

			void Allocate(size_t triangles);

			void Clear(void);

			size_t GetTrianglesCount(void) const;

			const VertexDeclaration& GetVertexDeclaration(void) const;

			const void* GetData(void) const;

			void SetVertexAttribute(size_t triangleIndex,
				size_t vertexIndex,
				VertexAttributeSemantic semantic,
				DataFormat type,
				const void *data);

			void GetVertexAttribute(size_t triangleIndex,
				size_t vertexIndex,
				VertexAttributeSemantic semantic,
				DataFormat type,
				void *data) const;

			VectorPair GetMinMaxVertices(void) const;

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			VertexData m_data;
			size_t m_triangles;

		};

		typedef std::shared_ptr<Mesh> MeshPointer;

	}

}
