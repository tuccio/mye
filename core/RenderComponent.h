#pragma once

#include "AlignedAllocator.h"

#include "Component.h"
#include "Material.h"
#include "Mesh.h"
#include "GPUBuffer.h"

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		class RenderComponent :
			public Component
		{

		public:

			__MYE_DECLARE_ALIGNED_16

			RenderComponent(void);
			RenderComponent(MeshPointer mesh, MaterialPointer material, const mye::math::Matrix4 & modelMatrix);
			~RenderComponent(void);

			bool IsVisible(void) const;
			void SetVisible(bool visible);

			//VertexDataPointer GetVertexData(void);

			MeshPointer GetMesh(void);
			void SetMesh(MeshPointer mesh);

			MaterialPointer GetMaterial(void) const;
			void SetMaterial(const MaterialPointer & material);

			const mye::math::AABB & GetBounds(void) const;
			void SetBounds(const mye::math::AABB & bounds);

			const mye::math::Matrix4 & GetModelMatrix(void) const;
			void SetModelMatrix(const mye::math::Matrix4 & matrix);

			RenderComponent * Clone(void) const;

			GPUBufferPointer GetGPUBuffer(void) const;
			
		private:

			bool m_visible;
			
			//VertexDataPointer m_vertexData;
			MeshPointer         m_mesh;
			GPUBufferPointer    m_gpuBuffer;

			MaterialPointer     m_material;
			
			mye::math::AABB     m_bounds;
			mye::math::Matrix4  m_modelMatrix;


			
		};

	}

}

