#pragma once

#include "Component.h"
#include "Material.h"
#include "Mesh.h"

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		class RenderComponent :
			public Component
		{

		public:

			RenderComponent(void);
			~RenderComponent(void);

			bool IsVisible(void) const;
			void SetVisible(bool visible);

			//VertexDataPointer GetVertexData(void);

			MeshPointer GetMesh(void);
			void SetMesh(MeshPointer mesh);

			Material* GetMaterialPointer(void);
			const Material& GetMaterial(void) const;
			void SetMaterial(const Material &material);

			const mye::math::AABB& GetBounds(void) const;
			void SetBounds(const mye::math::AABB &bounds);

			const mye::math::Matrix4& GetModelMatrix(void) const;
			void SetModelMatrix(const mye::math::Matrix4 &matrix);

			RenderComponent* Clone(void) const;
			
		private:

			bool m_visible;
			
			//VertexDataPointer m_vertexData;
			MeshPointer m_mesh;

			Material m_material;
			
			mye::math::AABB     m_bounds;
			mye::math::Matrix4  m_modelMatrix;


			
		};

	}

}

